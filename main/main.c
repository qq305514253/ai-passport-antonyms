#include "antonym_game.h"
#include "bsp_battery.h"
#include "bsp_button.h"
#include "bsp_display.h"
#include "bsp_i2c.h"
#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

LV_FONT_DECLARE(antonym_font_16);
LV_FONT_DECLARE(antonym_prompt_font_28);

#define INPUT_QUEUE_DEPTH 8

typedef struct {
    bsp_btn_t button;
    bsp_btn_ev_t event;
} input_event_t;

static const char *TAG = "antonym_practice";
static QueueHandle_t s_input_queue;
static antonym_game_t s_game;
static lv_obj_t *s_screen;
static lv_obj_t *s_battery;
static lv_obj_t *s_progress;
static lv_obj_t *s_prompt_caption;
static lv_obj_t *s_prompt;
static lv_obj_t *s_options[2];
static lv_obj_t *s_hint;

static void set_cn_font(lv_obj_t *object)
{
    lv_obj_set_style_text_font(object, &antonym_font_16, 0);
}

static lv_obj_t *make_label(lv_obj_t *parent, const char *text, uint32_t color)
{
    lv_obj_t *label = lv_label_create(parent);
    set_cn_font(label);
    lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
    lv_label_set_text(label, text);
    return label;
}

static void update_battery(void)
{
    int soc = bsp_battery_soc();
    if (soc >= 0) {
        lv_label_set_text_fmt(s_battery, "%d%%", soc);
    } else {
        lv_label_set_text(s_battery, "--%");
    }
}

static void style_option(lv_obj_t *option, bool selected, bool answered, bool correct)
{
    uint32_t background = 0xFFFFFF;
    uint32_t border = 0xB9C7D6;
    uint32_t text = 0x253858;

    if (answered && correct) {
        background = 0xDDF8E7;
        border = 0x2EA866;
        text = 0x175C37;
    } else if (answered && selected) {
        background = 0xFFE2E0;
        border = 0xE2544D;
        text = 0x842922;
    } else if (selected) {
        background = 0xDDEBFF;
        border = 0x3367D6;
        text = 0x173F86;
    }

    lv_obj_set_style_bg_color(option, lv_color_hex(background), 0);
    lv_obj_set_style_border_color(option, lv_color_hex(border), 0);
    lv_obj_set_style_border_width(option, (selected || (answered && correct)) ? 3 : 2, 0);
    lv_obj_set_style_shadow_color(option, lv_color_hex(border), 0);
    lv_obj_set_style_shadow_width(option, selected ? 8 : 0, 0);
    lv_obj_set_style_shadow_opa(option, selected ? LV_OPA_30 : LV_OPA_TRANSP, 0);
    lv_obj_set_style_text_color(lv_obj_get_child(option, 0), lv_color_hex(text), 0);
}

static void show_question(void)
{
    const antonym_question_t *question = antonym_game_question(&s_game);
    lv_label_set_text_fmt(s_progress, "%u / %u   %u分",
        (unsigned)(s_game.question_index + 1),
        (unsigned)ANTONYM_ROUND_LENGTH,
        (unsigned)(s_game.score * 10));
    lv_label_set_text(s_prompt_caption, "题目");
    lv_obj_set_style_text_font(s_prompt, &antonym_prompt_font_28, 0);
    lv_obj_set_style_text_color(s_prompt, lv_color_hex(0xFFF0A6), 0);
    lv_label_set_text(s_prompt, question->word);
    for (int i = 0; i < 2; ++i) {
        lv_label_set_text(lv_obj_get_child(s_options[i], 0), question->options[i]);
        style_option(s_options[i], i == s_game.selection, s_game.answered,
            i == question->correct_option);
    }
    lv_label_set_text(s_hint, s_game.answered
        ? (s_game.last_correct ? "答对啦！按确定继续" : "再想想，下次会更好！按确定继续")
        : "选择反义词：上/下切换，确定作答");
}

static void show_result(void)
{
    lv_label_set_text(s_progress, "本轮完成");
    lv_label_set_text(s_prompt_caption, "本轮");
    lv_obj_set_style_text_font(s_prompt, &antonym_font_16, 0);
    lv_obj_set_style_text_color(s_prompt, lv_color_hex(0xFFFFFF), 0);
    lv_label_set_text_fmt(s_prompt, "%u 分", (unsigned)(s_game.score * 10));
    lv_label_set_text(lv_obj_get_child(s_options[0], 0),
        s_game.score >= 8 ? "太棒了！" : "继续加油！");
    lv_label_set_text(lv_obj_get_child(s_options[1], 0), "长按确定再来一轮");
    style_option(s_options[0], true, false, false);
    style_option(s_options[1], false, false, false);
    lv_label_set_text(s_hint, "离线题库 · 不使用网络和 AI token");
}

static void refresh_ui(void)
{
    update_battery();
    if (s_game.state == ANTONYM_GAME_RESULT) {
        show_result();
    } else {
        show_question();
    }
}

static void build_ui(void)
{
    s_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(s_screen, lv_color_hex(0xF4F8FF), 0);
    lv_obj_set_style_pad_all(s_screen, 0, 0);

    lv_obj_t *header = lv_obj_create(s_screen);
    lv_obj_set_size(header, 240, 48);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x4467C4), 0);
    lv_obj_set_style_border_width(header, 0, 0);
    lv_obj_set_style_radius(header, 0, 0);

    lv_obj_t *title = make_label(header, "反义词小课堂", 0xFFFFFF);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 4, 0);
    s_battery = lv_label_create(header);
    lv_obj_set_style_text_font(s_battery, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(s_battery, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(s_battery, LV_ALIGN_RIGHT_MID, -2, 0);

    s_progress = make_label(s_screen, "", 0x526173);
    lv_obj_align(s_progress, LV_ALIGN_TOP_MID, 0, 59);

    lv_obj_t *question_card = lv_obj_create(s_screen);
    lv_obj_set_size(question_card, 190, 65);
    lv_obj_align(question_card, LV_ALIGN_TOP_MID, 0, 86);
    lv_obj_set_style_bg_color(question_card, lv_color_hex(0x5A4CC7), 0);
    lv_obj_set_style_border_color(question_card, lv_color_hex(0x33279E), 0);
    lv_obj_set_style_border_width(question_card, 3, 0);
    lv_obj_set_style_radius(question_card, 14, 0);
    lv_obj_set_style_pad_all(question_card, 0, 0);
    s_prompt_caption = make_label(question_card, "题目", 0xDAD7FF);
    lv_obj_align(s_prompt_caption, LV_ALIGN_TOP_MID, 0, 3);
    s_prompt = make_label(question_card, "", 0xFFF0A6);
    lv_obj_set_style_text_font(s_prompt, &antonym_prompt_font_28, 0);
    lv_obj_align(s_prompt, LV_ALIGN_BOTTOM_MID, 0, -3);

    for (int i = 0; i < 2; ++i) {
        s_options[i] = lv_obj_create(s_screen);
        lv_obj_set_size(s_options[i], 190, 48);
        lv_obj_align(s_options[i], LV_ALIGN_TOP_MID, 0, 164 + i * 58);
        lv_obj_set_style_radius(s_options[i], 12, 0);
        lv_obj_set_style_pad_all(s_options[i], 0, 0);
        lv_obj_t *label = make_label(s_options[i], "", 0x253858);
        lv_obj_center(label);
    }

    s_hint = make_label(s_screen, "", 0x526173);
    lv_obj_set_width(s_hint, 220);
    lv_obj_set_style_text_align(s_hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(s_hint, LV_ALIGN_BOTTOM_MID, 0, -12);

    refresh_ui();
    lv_screen_load(s_screen);
}

static void process_input(const input_event_t *input)
{
    if (input->event == BSP_BTN_LONG && input->button == BSP_BTN_OK) {
        antonym_game_start(&s_game);
    } else if (input->event == BSP_BTN_CLICK) {
        if (input->button == BSP_BTN_UP) {
            antonym_game_move(&s_game, -1);
        } else if (input->button == BSP_BTN_DOWN) {
            antonym_game_move(&s_game, 1);
        } else if (input->button == BSP_BTN_OK) {
            antonym_game_confirm(&s_game);
        }
    } else {
        return;
    }

    bsp_display_backlight(100);
    if (bsp_lvgl_lock(250)) {
        refresh_ui();
        bsp_lvgl_unlock();
    }
}

static void input_task(void *arg)
{
    (void)arg;
    input_event_t input;
    while (xQueueReceive(s_input_queue, &input, portMAX_DELAY) == pdTRUE) {
        process_input(&input);
    }
}

static void on_key(bsp_btn_t button, bsp_btn_ev_t event, void *user)
{
    (void)user;
    if (!s_input_queue) {
        return;
    }
    const input_event_t input = { .button = button, .event = event };
    (void)xQueueSend(s_input_queue, &input, 0);
}

void app_main(void)
{
    ESP_LOGI(TAG, "offline antonym practice starting");
    bsp_i2c_init();
    if (bsp_display_init() != ESP_OK || !bsp_lvgl_init()) {
        ESP_LOGE(TAG, "display initialization failed");
        return;
    }
    bsp_display_backlight(100);
    (void)bsp_battery_init();

    s_input_queue = xQueueCreate(INPUT_QUEUE_DEPTH, sizeof(input_event_t));
    if (!s_input_queue ||
        xTaskCreate(input_task, "antonym_input", 3072, NULL, 5, NULL) != pdPASS ||
        bsp_button_init(on_key, NULL) != ESP_OK) {
        ESP_LOGE(TAG, "input initialization failed");
        return;
    }

    antonym_game_start(&s_game);
    if (bsp_lvgl_lock(1000)) {
        build_ui();
        bsp_lvgl_unlock();
    }
}

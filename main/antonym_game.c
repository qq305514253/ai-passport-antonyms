#include "antonym_game.h"

static const antonym_question_t QUESTIONS[ANTONYM_ROUND_LENGTH] = {
    { "高", { "低", "远" }, 0 },
    { "大", { "多", "小" }, 1 },
    { "上", { "下", "前" }, 0 },
    { "左", { "右", "里" }, 0 },
    { "快", { "早", "慢" }, 1 },
    { "开", { "关", "进" }, 0 },
    { "黑", { "亮", "白" }, 1 },
    { "冷", { "热", "暖" }, 0 },
    { "长", { "短", "宽" }, 0 },
    { "好", { "真", "坏" }, 1 },
};

void antonym_game_start(antonym_game_t *game)
{
    *game = (antonym_game_t) {
        .state = ANTONYM_GAME_QUESTION,
        .question_index = 0,
        .selection = 0,
    };
}

void antonym_game_move(antonym_game_t *game, int direction)
{
    if (game->state != ANTONYM_GAME_QUESTION || game->answered || direction == 0) {
        return;
    }
    game->selection = game->selection == 0 ? 1 : 0;
}

void antonym_game_confirm(antonym_game_t *game)
{
    if (game->state != ANTONYM_GAME_QUESTION) {
        return;
    }
    if (!game->answered) {
        const antonym_question_t *question = antonym_game_question(game);
        game->last_correct = game->selection == question->correct_option;
        if (game->last_correct) {
            ++game->score;
        }
        game->answered = true;
        return;
    }

    ++game->question_index;
    game->selection = 0;
    game->answered = false;
    game->last_correct = false;
    if (game->question_index >= ANTONYM_ROUND_LENGTH) {
        game->state = ANTONYM_GAME_RESULT;
    }
}

const antonym_question_t *antonym_game_question(const antonym_game_t *game)
{
    size_t index = game->question_index;
    if (index >= ANTONYM_ROUND_LENGTH) {
        index = ANTONYM_ROUND_LENGTH - 1;
    }
    return &QUESTIONS[index];
}

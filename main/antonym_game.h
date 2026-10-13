#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ANTONYM_ROUND_LENGTH 10

typedef struct {
    const char *word;
    const char *options[2];
    uint8_t correct_option;
} antonym_question_t;

typedef enum {
    ANTONYM_GAME_QUESTION = 0,
    ANTONYM_GAME_RESULT,
} antonym_game_state_t;

typedef struct {
    antonym_game_state_t state;
    uint8_t question_index;
    uint8_t selection;
    uint8_t score;
    bool answered;
    bool last_correct;
} antonym_game_t;

void antonym_game_start(antonym_game_t *game);
void antonym_game_move(antonym_game_t *game, int direction);
void antonym_game_confirm(antonym_game_t *game);
const antonym_question_t *antonym_game_question(const antonym_game_t *game);

#include <assert.h>
#include <string.h>

#include "antonym_game.h"

int main(void)
{
    antonym_game_t game;
    antonym_game_start(&game);
    assert(game.state == ANTONYM_GAME_QUESTION);
    assert(strcmp(antonym_game_question(&game)->word, "高") == 0);

    antonym_game_move(&game, 1);
    assert(game.selection == 1);
    antonym_game_confirm(&game);
    assert(game.answered && !game.last_correct && game.score == 0);
    antonym_game_move(&game, 1);
    assert(game.selection == 1);
    antonym_game_confirm(&game);
    assert(game.question_index == 1 && !game.answered);

    for (int i = 1; i < ANTONYM_ROUND_LENGTH; ++i) {
        const antonym_question_t *question = antonym_game_question(&game);
        game.selection = question->correct_option;
        antonym_game_confirm(&game);
        antonym_game_confirm(&game);
    }
    assert(game.state == ANTONYM_GAME_RESULT);
    assert(game.score == ANTONYM_ROUND_LENGTH - 1);

    antonym_game_confirm(&game);
    assert(game.state == ANTONYM_GAME_RESULT);
    assert(game.score == ANTONYM_ROUND_LENGTH - 1);

    antonym_game_start(&game);
    assert(game.score == 0 && game.question_index == 0);
    return 0;
}

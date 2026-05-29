#ifndef H_LIBAI_TESTS
#define H_LIBAI_TESTS

#include <libai/mcts.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libutils/cmocka.h>

/* ai_play_turn */
void ai_play_turn_does_not_crash(void** state);
void ai_play_turn_completes_full_turn(void** state);
void ai_play_turn_advances_current_player(void** state);
void ai_play_turn_engine_ready_for_next_tile(void** state);
void ai_play_turn_many_iterations_does_not_crash(void** state);
void ai_play_turn_two_calls_advance_round(void** state);
void ai_play_turn_full_game_two_players(void** state);

static const struct CMUnitTest tests[] = {
    cmocka_unit_test(ai_play_turn_does_not_crash),
    cmocka_unit_test(ai_play_turn_completes_full_turn),
    cmocka_unit_test(ai_play_turn_advances_current_player),
    cmocka_unit_test(ai_play_turn_engine_ready_for_next_tile),
    cmocka_unit_test(ai_play_turn_many_iterations_does_not_crash),
    cmocka_unit_test(ai_play_turn_two_calls_advance_round),
    cmocka_unit_test(ai_play_turn_full_game_two_players),
};

#endif

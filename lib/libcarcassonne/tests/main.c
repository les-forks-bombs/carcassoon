#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>
#include <libcarcassonne/tests/tests.h>

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(deck_builds),
        cmocka_unit_test(deck_seed_consistent),
        cmocka_unit_test(game_builds),
        cmocka_unit_test(game_do_not_build_because_game_is_null),
        cmocka_unit_test(game_do_not_build_because_nb_players_too_low),
        cmocka_unit_test(game_do_not_build_because_nb_players_too_high),
        cmocka_unit_test(game_do_not_build_because_nb_ia_too_high),
        cmocka_unit_test(game_tile_at_works),
        cmocka_unit_test(game_tile_at_out_of_bounds),
        cmocka_unit_test(game_place_tile_works),
        cmocka_unit_test(game_place_tile_do_not_work_because_game_is_null),
        cmocka_unit_test(game_place_tile_do_not_work_because_tile_is_null),
        cmocka_unit_test(game_place_tile_do_not_work_because_tile_cannot_be_erased),
        cmocka_unit_test(game_place_tile_do_not_work_because_position_is_out_of_bounds),
        cmocka_unit_test(game_place_tile_do_not_work_because_position_is_taken),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

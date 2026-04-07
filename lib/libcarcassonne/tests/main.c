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
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

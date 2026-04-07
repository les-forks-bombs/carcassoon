#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <libcarcassonne/game.h>
#include <libcarcassonne/tests/tests.h>

/* Vérifie l'instanciation d'une game */
void game_builds(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);
    destroy_game(&game);
}

void game_do_not_build_because_game_is_null(void **state)
{
    assert_return_code(create_game(NULL, 3, 0, 500, 0), ERROR);
}

void game_do_not_build_because_nb_players_too_low(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 0, 0, 500, 0), ERROR);
}

void game_do_not_build_because_nb_players_too_high(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 6, 0, 500, 0), ERROR);
}

void game_do_not_build_because_nb_ia_too_high(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 4, 500, 0), ERROR);
}
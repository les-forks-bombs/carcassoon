#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <libcarcassonne/game.h>
#include <libcarcassonne/tests/tests.h>

/** create_game */

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

/** game_tile_at */

void game_tile_at_works(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    placed_tile_t **tile = game_tile_at(&game, -71, -71);
    assert_ptr_equal(tile, &game.map[0]);

    destroy_game(&game);
}

void game_tile_at_out_of_bounds(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    placed_tile_t **placed_tile = game_tile_at(&game, -200, 200);
    assert_ptr_equal(placed_tile, NULL);

    destroy_game(&game);
}

/** game_place_tile */

void game_place_tile_works(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    tile_t *tile = deck_pick(&game.deck);
    assert_ptr_not_equal(tile, NULL);

    assert_return_code(game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), SUCCESS);

    destroy_game(&game);
}

void game_place_tile_do_not_work_because_game_is_null(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    tile_t *tile = deck_pick(&game.deck);
    assert_ptr_not_equal(tile, NULL);

    assert_return_code(game_place_tile(NULL, tile, -200, 200, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), ERROR);

    destroy_game(&game);
}

void game_place_tile_do_not_work_because_tile_is_null(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    assert_return_code(game_place_tile(&game, NULL, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), ERROR);

    destroy_game(&game);
}

void game_place_tile_do_not_work_because_tile_cannot_be_erased(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    tile_t *tile = deck_pick(&game.deck);
    assert_ptr_not_equal(tile, NULL);

    assert_return_code(game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), SUCCESS);
    assert_return_code(game_place_tile(&game, NULL, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), NOT_FREE);

    destroy_game(&game);
}

void game_place_tile_do_not_work_because_position_is_out_of_bounds(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    tile_t *tile = deck_pick(&game.deck);
    assert_ptr_not_equal(tile, NULL);

    assert_return_code(game_place_tile(&game, tile, -200, 200, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), OUT_OF_BOUNDS);

    destroy_game(&game);
}

void game_place_tile_do_not_work_because_position_is_taken(void **state)
{
    game_t game;
    assert_return_code(create_game(&game, 3, 0, 500, 0), SUCCESS);

    tile_t *tile = deck_pick(&game.deck);
    assert_ptr_not_equal(tile, NULL);

    assert_return_code(game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), SUCCESS);
    assert_return_code(game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH), NOT_FREE);

    destroy_game(&game);
}
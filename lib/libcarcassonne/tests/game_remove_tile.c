#include <cmocka.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

void game_remove_tile_success(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  assert_non_null(*game_tile_at(&game, 0, 0));
  assert_int_equal(game_remove_tile(&game, 0, 0), SUCCESS);

  // La case doit être vide après retrait
  assert_null(*game_tile_at(&game, 0, 0));

  destroy_game(&game);
}

void game_remove_tile_empty_position_fails(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // Aucune tuile à (0, 0) → NO_TILE
  assert_int_equal(game_remove_tile(&game, 0, 0), NO_TILE);

  destroy_game(&game);
}

void game_remove_tile_out_of_bounds_fails(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  assert_int_equal(game_remove_tile(&game, -200, 200), OUT_OF_BOUNDS);

  destroy_game(&game);
}

void game_remove_tile_with_meeple_fails(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Placer un meeple dans le groupe B (ROAD, face ouest)
  player_t* player = &game.players[0];
  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      SUCCESS);

  // Retrait interdit si un meeple est présent
  assert_int_equal(game_remove_tile(&game, 0, 0), REMOVE_TILE_NOT_VOID);

  // Nettoyage : retirer le meeple avant destroy_game
  assert_int_equal(game_remove_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B),
                   SUCCESS);

  destroy_game(&game);
}

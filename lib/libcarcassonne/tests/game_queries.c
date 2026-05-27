#include <libcarcassonne/deck.h>
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tests/tests.h>

void game_is_place_open_true_with_empty_neighbors(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // (0,0) a 4 voisins libres → game_is_place_open doit renvoyer true
  assert_true(game_is_place_open(&game, 0, 0));

  destroy_game(&game);
}

void game_is_place_open_false_when_all_neighbors_occupied(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* t;

  // FRFR au centre : N=FIELD, E=ROAD, S=FIELD, W=ROAD
  t = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(t);
  assert_int_equal(
      game_place_tile(&game, t, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Voisin BAS (x+1) : besoin N=FIELD → FFFF convient
  t = deck_find_tile(&game.deck, "FFFF", false);
  assert_non_null(t);
  assert_int_equal(
      game_place_tile(&game, t, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Voisin HAUT (x-1) : besoin S=FIELD → FFFF convient
  t = deck_find_tile(&game.deck, "FFFF", false);
  assert_non_null(t);
  assert_int_equal(
      game_place_tile(&game, t, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Voisin DROITE (y+1) : besoin W=ROAD → FRFR convient
  t = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(t);
  assert_int_equal(
      game_place_tile(&game, t, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Voisin GAUCHE (y-1) : besoin E=ROAD → FRFR convient
  t = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(t);
  assert_int_equal(
      game_place_tile(&game, t, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // (0,0) est maintenant entouré → game_is_place_open doit renvoyer false
  assert_false(game_is_place_open(&game, 0, 0));

  destroy_game(&game);
}

void game_is_space_available_true_for_empty(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // Aucune tuile placée, (0, 0) est libre
  assert_true(game_is_space_available(&game, 0, 0));

  destroy_game(&game);
}

void game_is_space_available_false_when_occupied(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // (0,0) est occupé → plus disponible
  assert_false(game_is_space_available(&game, 0, 0));

  destroy_game(&game);
}

void game_is_tile_placeable_null_game_returns_false(void** state) {
  (void)state;
  const tile_t* tile = &LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0];
  assert_false(game_is_tile_placeable(NULL, tile, 0, 0,
                                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH));
}

void game_is_tile_placeable_null_tile_returns_false(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  assert_false(game_is_tile_placeable(&game, NULL, 0, 0,
                                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH));

  destroy_game(&game);
}

void game_get_current_player_returns_first_player(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  assert_ptr_equal(game_get_current_player(&game), &game.players[0]);

  destroy_game(&game);
}

void game_get_current_player_advances_after_end_turn(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  assert_ptr_equal(game_get_current_player(&game), &game.players[0]);
  assert_int_equal(game_end_player_turn(&game), SUCCESS);
  assert_ptr_equal(game_get_current_player(&game), &game.players[1]);

  destroy_game(&game);
}

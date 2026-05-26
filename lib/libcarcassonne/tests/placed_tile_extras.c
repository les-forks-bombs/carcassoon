#include <libcarcassonne/deck.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tests/tests.h>
#include <libcarcassonne/ext_base_game.h>

/// @brief Test que placed_tile_destroy gère NULL correctement
void placed_tile_destroy_handles_null(void** state) {
  (void)state;
  placed_tile_destroy(NULL);
}

/// @brief Test que placed_tile_create avec NULL retourne ERROR
void placed_tile_create_with_null(void** state) {
  (void)state;
  assert_int_equal(
      placed_tile_create(NULL, &LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0],
                         LIBCARCASSONNE_TILE_ORIENTATION_NORTH, 0, 0),
      ERROR);
}

/// @brief Test placed_tile_group_connected avec des tuiles FRRR liées
/// FRRR: parts_groups = {A, A, A, B, G, C, E, D, F}
///       parts = {FIELD, FIELD, FIELD, ROAD, VILLAGE, ROAD, FIELD, ROAD, FIELD}
/// Orientation NORTH:
///   - Face OUEST (position 3) = groupe B (ROAD)
///   - Face EST (position 5) = groupe C (ROAD)
///   - Face SUD (position 7) = groupe D (ROAD)
/// Quand on place FRRR à (0,0) et FRRR à (0,1):
///   - (0,0) face EST (droite, position 5) = groupe C
///   - (0,1) face OUEST (gauche, position 3) = groupe B
///   - Ces deux groupes devraient être connectés automatiquement
void placed_tile_group_connected_with_linked_frrr_tiles(void** state) {
  (void)state;

  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // FRRR a 4 exemplaires
  const tile_t* tile1 = deck_find_tile(&game.deck, "FRRR", false);
  assert_ptr_not_equal(tile1, NULL);
  assert_int_equal(
      game_place_tile(&game, tile1, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  const tile_t* tile2 = deck_find_tile(&game.deck, "FRRR", false);
  assert_ptr_not_equal(tile2, NULL);
  assert_int_equal(
      game_place_tile(&game, tile2, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t** p1 = game_tile_at(&game, 0, 0);
  placed_tile_t** p2 = game_tile_at(&game, 0, 1);
  assert_non_null(p1);
  assert_non_null(*p1);
  assert_non_null(p2);
  assert_non_null(*p2);

  // FRRR face EST (position 5) = groupe C
  // FRRR face OUEST (position 3) = groupe B
  placed_tile_group_t* road_p1_east = (*p1)->groups[C];
  placed_tile_group_t* road_p2_west = (*p2)->groups[B];

  assert_true(placed_tile_group_connected(road_p1_east, road_p2_west));

  destroy_game(&game);
}


/// @brief Test placed_tile_group_eval_points pour une route simple
/// Une route ROAD vaut 1 point par tuile (comportement actuel du code)
void placed_tile_group_eval_points_single_road(void** state) {
  (void)state;

  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t** p = game_tile_at(&game, 0, 0);
  assert_non_null(p);
  assert_non_null(*p);

  // FRRR: Face OUEST = position 3 = groupe B = ROAD
  placed_tile_group_t* road_group = (*p)->groups[B];
  assert_non_null(road_group);
  
  placed_tile_group_eval_points_t eval =
      placed_tile_group_eval_points(road_group, false);
  
  // Une route vaut 1 point par tuile (comportement actuel)
  assert_int_equal(eval.points, 1);

  destroy_game(&game);
}

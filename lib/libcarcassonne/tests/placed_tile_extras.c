
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

void placed_tile_destroy_handles_null(void** state) {
  (void)state;
  placed_tile_destroy(NULL);
}

void placed_tile_create_with_null(void** state) {
  (void)state;
  assert_int_equal(
      placed_tile_create(NULL, &LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0],
                         LIBCARCASSONNE_TILE_ORIENTATION_NORTH, 0, 0),
      ERROR);
}

void placed_tile_group_connected_with_linked_frrr_tiles(void** state) {
  (void)state;

  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile1 = deck_find_tile(&game.deck, "FRRR", false);
  assert_ptr_not_equal(tile1, NULL);
  assert_int_equal(game_place_tile(&game, tile1, 0, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  const tile_t* tile2 = deck_find_tile(&game.deck, "FRRR", false);
  assert_ptr_not_equal(tile2, NULL);
  assert_int_equal(game_place_tile(&game, tile2, 0, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  placed_tile_t** p1 = game_tile_at(&game, 0, 0);
  placed_tile_t** p2 = game_tile_at(&game, 0, 1);
  assert_non_null(p1);
  assert_non_null(*p1);
  assert_non_null(p2);
  assert_non_null(*p2);

  placed_tile_group_t* road_p1_east = (*p1)->groups[LIBCARCASSONNE_TILE_PART_C];
  placed_tile_group_t* road_p2_west = (*p2)->groups[LIBCARCASSONNE_TILE_PART_B];

  assert_true(placed_tile_group_connected(road_p1_east, road_p2_west));

  destroy_game(&game);
}

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

  placed_tile_group_t* road_group = (*p)->groups[LIBCARCASSONNE_TILE_PART_B];
  assert_non_null(road_group);

  placed_tile_group_eval_points_t eval =
      placed_tile_group_eval_points(road_group, false);

  assert_int_equal(eval.points, 1);

  destroy_game(&game);
}

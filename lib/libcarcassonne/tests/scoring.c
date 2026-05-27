
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

#include "libutils/vector.h"

// FRFR : parts_groups = {A,A,A,B,D,B,C,C,C}
//   Groupe B = ROAD (faces ouest+est, positions 3 et 5)
//
// CFFF : parts_groups = {A,B,A,A,C,A,A,A,A}, blason=false
//   Groupe B = TOWN (face nord, position 1, open_slots=1)
//
// CCCC : parts_groups = {A,A,A,A,B,A,A,A,A}, blason=true
//   Groupe A = TOWN (faces 1,3,5,7, open_slots=4)
//
// FFFF : parts_groups = {A,A,A,A,B,A,A,A,A}, blason=false
//   Groupe A = FIELD (faces 1,3,5,7)
//   Groupe B = ABBEY (centre, position 4)
//
// FRRR : parts_groups = {A,A,A,B,G,C,E,D,F}
//   Groupe G = VILLAGE (centre, position 4)

void scoring_road_two_tiles_two_points(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // Route droite W-E : FRFR à (0,0) puis FRFR à (0,1) (voisin EST)
  const tile_t* t1 = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(t1);
  assert_int_equal(
      game_place_tile(&game, t1, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  const tile_t* t2 = deck_find_tile(&game.deck, "FRFR", false);
  assert_non_null(t2);
  assert_int_equal(
      game_place_tile(&game, t2, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Groupe B de (0,0) est lié au groupe B de (0,1) → 2 nœuds, 2 points
  placed_tile_t** p = game_tile_at(&game, 0, 0);
  assert_non_null(*p);
  placed_tile_group_t* road = (*p)->groups[LIBCARCASSONNE_TILE_PART_B];
  placed_tile_group_eval_points_t eval =
      placed_tile_group_eval_points(road, false);

  assert_int_equal(eval.points, 2);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

void scoring_town_incomplete_one_point(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "CFFF", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // CFFF : groupe B = TOWN, pas de blason, non complétée
  placed_tile_t**                 p    = game_tile_at(&game, 0, 0);
  placed_tile_group_eval_points_t eval = placed_tile_group_eval_points(
      (*p)->groups[LIBCARCASSONNE_TILE_PART_B], false);

  assert_int_equal(eval.points, 1);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

void scoring_town_with_blason_incomplete(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // CCCC : blason=true → 1+1=2 points par tuile (non complétée)
  const tile_t* tile = deck_find_tile(&game.deck, "CCCC", true);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t**                 p    = game_tile_at(&game, 0, 0);
  placed_tile_group_eval_points_t eval = placed_tile_group_eval_points(
      (*p)->groups[LIBCARCASSONNE_TILE_PART_A], false);

  assert_int_equal(eval.points, 2);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

void scoring_town_complete_flag_doubles_value(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  // CCCC blason=true, évaluation is_completed=true : (1+1)*2 = 4 points
  const tile_t* tile = deck_find_tile(&game.deck, "CCCC", true);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  placed_tile_t**                 p    = game_tile_at(&game, 0, 0);
  placed_tile_group_eval_points_t eval = placed_tile_group_eval_points(
      (*p)->groups[LIBCARCASSONNE_TILE_PART_A], true);

  assert_int_equal(eval.points, 4);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

void scoring_field_zero_points(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FFFF", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // FFFF : groupe A = FIELD → 0 point
  placed_tile_t**                 p    = game_tile_at(&game, 0, 0);
  placed_tile_group_eval_points_t eval = placed_tile_group_eval_points(
      (*p)->groups[LIBCARCASSONNE_TILE_PART_A], false);

  assert_int_equal(eval.points, 0);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

void scoring_village_zero_points(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // FRRR : groupe G = VILLAGE (centre, position 4) → 0 point
  placed_tile_t**                 p    = game_tile_at(&game, 0, 0);
  placed_tile_group_eval_points_t eval = placed_tile_group_eval_points(
      (*p)->groups[LIBCARCASSONNE_TILE_PART_G], false);

  assert_int_equal(eval.points, 0);
  vector_free(&eval.meeples);

  destroy_game(&game);
}

#include <assert.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/tests/tests.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "libcarcassonne/action.h"
#include "libcarcassonne/consts.h"
#include "libcarcassonne/deck.h"
#include "libcarcassonne/ext_base_game.h"
#include "libcarcassonne/game.h"
#include "libcarcassonne/meeple.h"
#include "libcarcassonne/tile.h"
#include "libutils/vector.h"

void engine_builds(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);
  destroy_engine(&engine);
}

void engine_short_play_test(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  const tile_t* tile;

  tile = deck_find_tile(&engine.game.deck, "FCFC", true);
  assert_ptr_not_equal(tile, NULL);
  action_t action = {
      .type  = LIBCARCASSONNE_ACTION_PLACE_TILE,
      .order = {.place_tile = {
                    .tile        = tile,
                    .x           = -1,
                    .y           = 0,
                    .orientation = LIBCARCASSONNE_TILE_ORIENTATION_WEST}}};

  // assert_int_equal(engine.current_hook, 0);
  // assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
  // assert_int_equal(engine.current_hook, 0);

  assert_int_equal(start_game(&engine), SUCCESS);

  assert_int_equal(engine.current_hook, 0);
  assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
  assert_int_equal(engine.current_hook, 1);

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  placed_tile_t** placed_tile = game_tile_at(&engine.game, -1, 0);

  assert_non_null(placed_tile);
  assert_non_null(*placed_tile);

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = B;
  action.order.place_meeple.tile        = *placed_tile;
  action.order.place_meeple.x           = -1;
  action.order.place_meeple.y           = 0;

  action_vector_t meeple_actions = engine_get_actions(&engine);

  assert_int_equal(vector_size(&meeple_actions), 4);

  action_t correct_place_meeple_actions[] = {
      // CHAMP GAUCHE
      {.order.place_meeple = {.tile        = *placed_tile,
                              .part_group  = A,
                              .meeple_type = BASIC,
                              .x           = -1,
                              .y           = 0},
       .type               = LIBCARCASSONNE_ACTION_PLACE_MEEPLE},
      // VILLE
      {.order.place_meeple = {.tile        = *placed_tile,
                              .part_group  = B,
                              .meeple_type = BASIC,
                              .x           = -1,
                              .y           = 0},
       .type               = LIBCARCASSONNE_ACTION_PLACE_MEEPLE},
      // CHAMP DROIT
      {.order.place_meeple = {.tile        = *placed_tile,
                              .part_group  = C,
                              .meeple_type = BASIC,
                              .x           = -1,
                              .y           = 0},
       .type               = LIBCARCASSONNE_ACTION_PLACE_MEEPLE},
      // NONE
      {.order = {0}, .type = LIBCARCASSONNE_ACTION_NONE},
  };

  for (unsigned int i = 0; i < vector_size(&meeple_actions); i++) {
    action_t a1 = *vector_nth(&meeple_actions, i);
    action_t a2 = correct_place_meeple_actions[i];

    assert_int_equal(a1.order.place_tile.x, a2.order.place_tile.x);
    assert_int_equal(a1.order.place_tile.y, a2.order.place_tile.y);
    assert_int_equal(a1.order.place_tile.orientation,
                     a2.order.place_tile.orientation);
    assert_ptr_equal(a1.order.place_tile.tile, a2.order.place_tile.tile);
    assert_int_equal(a1.type, a2.type);
  }

  vector_free(&meeple_actions);

  assert_int_equal(dispatch_action(&engine, action), SUCCESS);
  assert_int_equal(engine.current_hook, 0);

  action_vector_t tile_actions = engine_get_actions(&engine);

  tile = deck_find_tile(&engine.game.deck, "CFCF", false);
  action_t correct_place_tile_actions[] = {
      // -1, 1 : HAUT DROIT
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                            .x = -1,
                            .y = 1},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                            .x = -1,
                            .y = 1},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      // -1, -1 : HAUT GAUCHE
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                            .x = -1,
                            .y = -1},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                            .x = -1,
                            .y = 1},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      // -2, 0 : HAUT HAUT
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                            .x = -2,
                            .y = 0},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      {.order.place_tile = {.tile = tile,
                            .orientation =
                                LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                            .x = -2,
                            .y = 0},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      // 1, 0 : BAS
      {.order.place_tile = {.tile        = tile,
                            .orientation = LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                            .x           = 1,
                            .y           = 0},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE},
      {.order.place_tile = {.tile        = tile,
                            .orientation = LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                            .x           = 1,
                            .y           = 0},
       .type             = LIBCARCASSONNE_ACTION_PLACE_TILE}};

  for (unsigned int i = 0; i < vector_size(&tile_actions); i++) {
    action_t a1 = *vector_nth(&tile_actions, i);
    action_t a2 = correct_place_tile_actions[i];

    assert_int_equal(a1.order.place_tile.x, a2.order.place_tile.x);
    assert_int_equal(a1.order.place_tile.y, a2.order.place_tile.y);
    assert_int_equal(a1.order.place_tile.orientation,
                     a2.order.place_tile.orientation);
    assert_ptr_equal(a1.order.place_tile.tile, a2.order.place_tile.tile);
    assert_int_equal(a1.type, a2.type);
  }

  vector_free(&tile_actions);

  engine_revert(&engine, 0);

  assert_int_equal(engine.current_hook, 0);
  for (int x = -2; x < 3; x++) {
    for (int y = -2; y < 3; y++) {
      placed_tile_t* tile = *game_tile_at(&engine.game, x, y);

      if (x == 0 && y == 0) {
        assert_non_null(tile);
        assert_ptr_equal(tile->parent,
                         &LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS[0]);
      } else {
        assert_null(tile);
      }
    }
  }

  destroy_engine(&engine);
}

void engine_long_play_test(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* tile;
  action_t      action = {0};

  // 10 tours avec tuiles, positions, orientations et part_groups valides
  // Basé sur l'ancien long_play_test qui fonctionnait
  struct {
    const char* tile_id;
    bool        blason;
    int         x, y, orientation;
    int         part_group;
  } turns[] = {// Tour 1 - 3 joueurs
               {"FCFC", true, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B},
               {"FRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C},
               {"FFRR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH, A},
               // Tour 2
               {"FRFR", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, A},
               {"CRFR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, D},
               {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B},
               // Tour 3
               {"RRRR", false, -2, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C},
               {"CCRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C},
               {"CFRR", false, 1, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST, C},
               // Tour 4
               {"CRRR", false, -2, 2, LIBCARCASSONNE_TILE_ORIENTATION_WEST, A}};

  for (int i = 0; i < 10; i++) {
    if (i != 0) {
      int         size      = vector_size(&engine.dispatchs);
      dispatch_t* next_turn = vector_nth(&engine.dispatchs, size - 1);
      assert_ptr_equal(next_turn->hook, &hook_next_player);
    }

    tile = deck_find_tile(&engine.game.deck, turns[i].tile_id, turns[i].blason);
    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 0);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 1);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = BASIC;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    assert_int_equal(engine.current_hook, 0);

    vector_free(&meeple_actions);
  }

  destroy_engine(&engine);
}
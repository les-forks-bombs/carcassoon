#include "libcarcassonne/engine.h"

#include <assert.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/vector.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "libcarcassonne/deck.h"
#include "libcarcassonne/enums.h"
#include "libcarcassonne/extensions_list.h"
#include "libcarcassonne/forward.h"
#include "libcarcassonne/macros.h"
#include "libutils/lc.h"

void engine_builds(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);
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
    const char*   tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {// Tour 1 - 3 joueurs
               {"FCFC", true, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_B, BASIC},
               {"FRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC},
               {"FFRR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                LIBCARCASSONNE_TILE_PART_A, BASIC},
               // Tour 2
               {"FRFR", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_A, BASIC},
               {"CRFR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_B, BASIC},
               // Tour 3
               {"RRRR", false, -2, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC},
               {"CCRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC},
               {"CFRR", false, 1, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_C, NONE},
               // Tour 4
               {"CRRR", false, -2, 2, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_A, BASIC}};

  for (int i = 0; i < 10; i++) {
    if (i != 0) {
      int size = vector_size(&engine.dispatchs);
      assert_true(size > 0);
      dispatch_t* last_hook = vector_nth(&engine.dispatchs, size - 1);
      assert_ptr_equal(last_hook->hook, &hook_pick_tile);
    }

    tile = deck_find_tile(&engine.game.deck, turns[i].tile_id, turns[i].blason);
    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 1);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 2);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = turns[i].meeple_type;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    assert_int_equal(engine.current_hook, 1);

    vector_free(&meeple_actions);
  }

  destroy_engine(&engine);
}

void engine_trigger_end_game(void** state) {
  (void)state;
  engine_t engine   = {0};
  options.max_turns = 4;
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  assert_int_equal(start_game(&engine), SUCCESS);

  free_deck(engine.game.deck);

  memset(&engine.game.deck.list.meta, 0, sizeof(list_t));

  // 10 tours avec tuiles, positions, orientations et part_groups valides
  // Basé sur l'ancien long_play_test qui fonctionnait
  struct {
    const char*   tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
    const tile_t* tile;
  } turns[] = {// Tour 1 - 3 joueurs
               {"FCFC", true, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_B, BASIC, NULL},
               {"FRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC, NULL},
               {"FFRR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                LIBCARCASSONNE_TILE_PART_A, BASIC, NULL},
               // Tour 2
               {"FRFR", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_A, BASIC, NULL},
               {"CRFR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_A, BASIC, NULL},
               {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_B, BASIC, NULL},
               // Tour 3
               {"RRRR", false, -2, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC, NULL},
               {"CCRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_C, BASIC, NULL},
               {"CFRR", false, 1, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_C, BASIC, NULL},
               // Tour 4
               {"CRRR", false, -2, 2, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                LIBCARCASSONNE_TILE_PART_A, BASIC, NULL},
               {"CCRR", true, -1, 2, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                LIBCARCASSONNE_TILE_PART_B, BASIC, NULL},
               {"CCFF", true, 0, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_B, NONE, NULL}};

  for (unsigned int i = 0; i < SIZE(turns); i++) {
    for (unsigned int k = 0; k < LIBCARCASSONNE_EXTENSIONS[0]->tiles->meta.size;
         k++) {
      const tile_t* tile = vector_nth(LIBCARCASSONNE_EXTENSIONS[0]->tiles, k);
      if (strcmp(tile->family, turns[i].tile_id) == 0 &&
          tile->blason == turns[i].blason) {
        list_append(&engine.game.deck.list, &tile);
        turns[i].tile = tile;
      }
    }
  }

  for (unsigned int i = 0; i < SIZE(turns); i++) {
    if (i != 0) {
      unsigned int size = vector_size(&engine.dispatchs);
      assert_true(size > 0);
      dispatch_t* last_hook = vector_nth(&engine.dispatchs, size - 1);
      assert_ptr_equal(last_hook->hook, &hook_pick_tile);
    }

    const tile_t* tile = turns[i].tile;

    action_vector_t actions = engine_get_actions(&engine);
    action_t        action;

    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 1);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 2);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = turns[i].meeple_type;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    if (i == SIZE(turns) - 1) {
      assert_int_equal(engine.current_hook, 0);
    } else {
      assert_int_equal(engine.current_hook, 1);
    }

    vector_free(&actions);

    vector_free(&meeple_actions);
  }

  assert_true(is_game_finished(&engine.game));
  assert_int_equal(engine.game.turn, 4);
  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  assert_int_equal(engine.game.players[0].score, 3 + 2 + 1);
  assert_int_equal(engine.game.players[1].score, 4 + 5 + 16);
  assert_int_equal(engine.game.players[2].score, 16 + 1);

  destroy_engine(&engine);
}

void engine_trigger_give_back_meeple(void** state) {
  (void)state;

  unsigned int max_turns = 2;

  engine_t engine   = {0};
  options.max_turns = max_turns;
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* tile;
  action_t      action = {0};

  struct {
    const char*   tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {
      // Tour 1 - 3 joueurs
      {"CRRR", false, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
       LIBCARCASSONNE_TILE_PART_A, BASIC},
      {"RRRR", false, -1, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
       LIBCARCASSONNE_TILE_PART_C, BASIC},
      {"FRRR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
       LIBCARCASSONNE_TILE_PART_B, BASIC},
      {"CFFF", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
       LIBCARCASSONNE_TILE_PART_B, BASIC},
      {"CRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
       LIBCARCASSONNE_TILE_PART_A, BASIC},
      {"CCRR", true, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, 0, NONE},
  };

  for (unsigned int i = 0; i < SIZE(turns); i++) {
    if (i != 0) {
      unsigned int size = vector_size(&engine.dispatchs);
      assert_true(size > 0);
      dispatch_t* last_hook = vector_nth(&engine.dispatchs, size - 1);
      assert_ptr_equal(last_hook->hook, &hook_pick_tile);
    }

    tile = deck_find_tile(&engine.game.deck, turns[i].tile_id, turns[i].blason);
    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 1);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 2);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = turns[i].meeple_type;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    if (i == SIZE(turns) - 1) {
      assert_int_equal(engine.current_hook, 0);
    } else {
      assert_int_equal(engine.current_hook, 1);
    }

    vector_free(&meeple_actions);
  }

  assert_true(is_game_finished(&engine.game));
  assert_int_equal(engine.game.turn, max_turns);
  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  assert_int_equal(engine.game.players[0].score, 4 + 8);
  assert_int_equal(engine.game.players[1].score, 2 + 8);
  assert_int_equal(engine.game.players[2].score, 2);

  destroy_engine(&engine);
}

void engine_abbey_completed(void** state) {
  (void)state;

  engine_t engine   = {0};
  options.max_turns = 3;
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* tile;
  action_t      action = {0};

  // Scenario : Joueur 0 place une abbaye (FFFF) à (2,2) avec un meeple BASIC
  // Puis les 8 tuiles autour sont placées pour compléter l'abbaye
  // Note: (0,0) est déjà occupée par start_game, mais ce n'est pas un problème
  // L'abbaye complétée (8 tuiles autour) rapporte 9 points
  struct {
    const char*   tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {// Tour 1 : 3 joueurs
               {"FFFF", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_B, BASIC},
               {"FRFR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"FRFR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               // Tour 2 : 3 joueurs
               {"FRFR", false, 2, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"FRFR", false, 2, -1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"CFFF", false, 2, 0, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               // Tour 3 : 3 joueurs
               {"FCFC", false, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                LIBCARCASSONNE_TILE_PART_A, NONE},
               {"CCRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                LIBCARCASSONNE_TILE_PART_A, NONE}};

  for (unsigned int i = 0; i < SIZE(turns); i++) {
    if (i != 0) {
      unsigned int size = vector_size(&engine.dispatchs);
      assert_true(size > 0);
      dispatch_t* last_hook = vector_nth(&engine.dispatchs, size - 1);
      assert_ptr_equal(last_hook->hook, &hook_pick_tile);
    }

    tile = deck_find_tile(&engine.game.deck, turns[i].tile_id, turns[i].blason);
    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 1);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 2);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    // Placement du meeple (ou NONE pour passer)
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = turns[i].meeple_type;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    if (i == SIZE(turns) - 1) {
      assert_int_equal(engine.current_hook, 0);
    } else {
      assert_int_equal(engine.current_hook, 1);
    }

    vector_free(&meeple_actions);
  }

  assert_true(is_game_finished(&engine.game));
  assert_int_equal(engine.game.turn, 3);
  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  // L'abbaye est complétée (8 tuiles autour), donc 9 points pour le joueur 0
  assert_int_equal(engine.game.players[0].score, 9);
  // Les autres joueurs n'ont pas posé de meeple sur des groupes completés
  assert_int_equal(engine.game.players[1].score, 0);
  assert_int_equal(engine.game.players[2].score, 0);

  destroy_engine(&engine);
}

void engine_abbey_incomplete(void** state) {
  (void)state;

  engine_t engine   = {0};
  options.max_turns = 1;
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* tile;
  action_t      action = {0};

  // Scenario : Joueur 0 place une abbaye (FFFF) à (1,1) avec un meeple BASIC
  // Seulement 2 tuiles sont placées autour avant la fin de partie
  // Note: (0,0) est déjà occupée par start_game
  // L'abbaye a 2 tuiles autour (nord et est) → 2 points pour le joueur 0
  struct {
    const char*   tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {
      // Tour 1 : 3 joueurs
      // Joueur 0 place FFFF (abbaye au centre) à (1,1) et pose un meeple
      {"FFFF", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
       LIBCARCASSONNE_TILE_PART_B, BASIC},
      // Joueur 1 place une tuile à (1,0) - nord
      {"FCFC", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
       LIBCARCASSONNE_TILE_PART_A, NONE},
      // Joueur 2 place une tuile à (2,1) - est
      {"FFRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
       LIBCARCASSONNE_TILE_PART_A, NONE}};

  for (size_t i = 0; i < SIZE(turns); i++) {
    tile = deck_find_tile(&engine.game.deck, turns[i].tile_id, turns[i].blason);
    assert_ptr_not_equal(tile, NULL);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    assert_int_equal(engine.current_hook, 1);
    assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
    assert_int_equal(engine.current_hook, 2);

    // Vérification des actions de meeple disponibles
    action_vector_t meeple_actions = engine_get_actions(&engine);
    assert_int_not_equal(vector_size(&meeple_actions), 0);

    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t** placed_tile =
        game_tile_at(&engine.game, turns[i].x, turns[i].y);
    assert_non_null(placed_tile);
    assert_non_null(*placed_tile);

    action.order.place_meeple.meeple_type = turns[i].meeple_type;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    assert_int_equal(dispatch_action(&engine, action), SUCCESS);
    if (i == SIZE(turns) - 1) {
      assert_int_equal(engine.current_hook, 0);
    } else {
      assert_int_equal(engine.current_hook, 1);
    }

    vector_free(&meeple_actions);
  }

  assert_int_equal(engine.game.turn, 1);
  assert_true(is_game_finished(&engine.game));
  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  // L'abbaye a 3 tuiles autour, donc 4 points pour le joueur 0
  assert_int_equal(engine.game.players[0].score, 4);
  // Les autres joueurs n'ont pas posé de meeple
  assert_int_equal(engine.game.players[1].score, 0);
  assert_int_equal(engine.game.players[2].score, 0);

  destroy_engine(&engine);
}

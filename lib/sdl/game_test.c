#include <sdl/game_test.h>
#include <sdl/consts.h>
#include "libcarcassonne/engine.h"
#include "libcarcassonne/ext_base_game.h"
#include "libcarcassonne/forward.h"
#include "libutils/vector.h"

void init_game(AppState *as) {
  const tile_t *tile;
  action_t      action = {0};

  // 10 tours avec tuiles, positions, orientations et part_groups valides
  // Basé sur l'ancien long_play_test qui fonctionnait
  struct {
    const char   *tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {
      // Tour 1 - 3 joueurs
      {"FCFC", true, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B, BASIC},
      {"FRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"FFRR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH, A, BASIC},
      // Tour 2
      {"FRFR", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, A, BASIC},
      {"CRFR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, D, NONE},
      {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B, BASIC},
      // Tour 3
      {"RRRR", false, -2, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"CCRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"CFRR", false, 1, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST, C, NONE},
      // Tour 4
      {"CRRR", false, -2, 2, LIBCARCASSONNE_TILE_ORIENTATION_WEST, A, BASIC}};

  for (int i = 0; i < 10; i++) {
    tile = deck_find_tile(&as->engine.game.deck, turns[i].tile_id,
                          turns[i].blason);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    dispatch_action(&as->engine, action);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t **placed_tile =
        game_tile_at(&as->engine.game, turns[i].x, turns[i].y);

    action.order.place_meeple.meeple_type = BASIC;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    dispatch_action(&as->engine, action);

    action_vector_t actions = engine_get_actions(&as->engine);
    as->next_action=vector_nth(&actions, 0);
    as->is_waiting_for_tile=true;
  }
  
}
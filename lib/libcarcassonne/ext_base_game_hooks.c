#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/game.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libcarcassonne/dispatch.h"
#include "libcarcassonne/forward.h"
#include "libutils/vector.h"

return_code_t meeple_place_fw(void **state_store, engine_t *engine,
                              action_t *action) {
  if (action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE) {
    *state_store = malloc(sizeof(meeple_place_hook_state_t));
    meeple_place_hook_state_t *state = *state_store;

    state->x           = action->order.place_meeple.x;
    state->y           = action->order.place_meeple.y;
    state->group       = action->order.place_meeple.part_group;
    state->meeple_type = action->order.place_meeple.meeple_type;

    // on place le meeple
    game_place_meeple(&engine->game, state->x, state->y, state->group,
                      action->order.place_meeple.meeple_type);

    (vector_nth(
         &engine->game.players[engine->game.current_player].meeples_count,
         state->meeple_type))
        ->count--;

    return SUCCESS;
  } else {
    engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION;
    return NO_PROGRESS;
  }
}

return_code_t meeple_place_bw(void **state_store, engine_t *engine) {
  meeple_place_hook_state_t *state = *state_store;

  // game_remove_meeple(&engine->game, state->x, state->y, state->group);
  (vector_nth(&engine->game.players[engine->game.current_player].meeples_count,
              state->meeple_type))
      ->count++;

  free(state);

  return SUCCESS;
}

return_code_t tile_place_fw(void **state_store, engine_t *engine,
                            action_t *action) {
  *state_store                   = malloc(sizeof(tile_place_hook_state_t));
  tile_place_hook_state_t *state = *state_store;

  state->x = action->order.place_tile.x;
  state->y = action->order.place_tile.y;

  // on place la tile
  game_place_tile(&engine->game, action->order.place_tile.tile, state->x,
                  state->y, action->order.place_tile.orientation);

  return SUCCESS;
}

return_code_t tile_place_bw(void **state_store, engine_t *engine) {
  tile_place_hook_state_t *state = *state_store;
  free(state);
  // todo: retirer une tile du jeu

  return SUCCESS;
}

return_code_t rendre_meeples_fw(void **state_store, engine_t *engine,
                                action_t *action) {
  // on récupère la dernière action "place"
  tile_place_hook_state_t *place = NULL;
  for (unsigned j = vector_size(&engine->dispatchs); j != 0; j--) {
    dispatch_t *dispatch = vector_nth(&engine->dispatchs, j);

    if (dispatch->hook == &hook_tile_place) {
      place = (tile_place_hook_state_t *)dispatch->action;
      break;
    }
  }

  if (place == NULL) {
    return SUCCESS;
  }

  placed_tile_t **tile = game_tile_at(&engine->game, place->x, place->y);
  if (tile == NULL) {
    return SUCCESS;
  }
  if (*tile == NULL) {
    return SUCCESS;
  }

  placed_tile_t *placed_tile = *tile;

  // pour chacun des groupes de la tile placée, si il y a des meeples, on les
  // retire
  bool visite[9];
  memset(&visite, 0, sizeof(visite));

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!visite[group]) {
      meeple_t *meeple = placed_tile->groups[group]->meeple;
      // pour chaque groupe, si un meeple est placé
      if (meeple != NULL) {
        vector_remove_value(&meeple->player->meeples, &meeple);

        (*vector_nth(&meeple->player->meeples_count, meeple->meeple_type))
            .count++;

        // retirer le meeple
        meeple = NULL;
        free(meeple);
      }
    }
    visite[group] = true;
  }
}

return_code_t rendre_meeples_bw(void **state_store, engine_t *engine) {
  // annulation des meeples rendus

  free(*state_store);
}

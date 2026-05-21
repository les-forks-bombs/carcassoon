#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/game.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libcarcassonne/deck.h"
#include "libcarcassonne/dispatch.h"
#include "libcarcassonne/forward.h"
#include "libcarcassonne/meeple.h"
#include "libcarcassonne/placed_tile.h"
#include "libcarcassonne/player.h"
#include "libcarcassonne/tile.h"
#include "libutils/lc.h"
#include "libutils/vector.h"

return_code_t meeple_place_fw(void **state_store, engine_t *engine,
                              action_t *action) {
  *state_store                     = malloc(sizeof(meeple_place_hook_state_t));
  meeple_place_hook_state_t *state = *state_store;

  state->x           = action->order.place_meeple.x;
  state->y           = action->order.place_meeple.y;
  state->group       = action->order.place_meeple.part_group;
  state->meeple_type = action->order.place_meeple.meeple_type;

  // on place le meeple
  game_place_meeple(&engine->game, state->x, state->y, state->group,
                    action->order.place_meeple.meeple_type,
                    game_get_current_player(&engine->game));

  return SUCCESS;
}

return_code_t meeple_place_bw(void **state_store, engine_t *engine) {
  meeple_place_hook_state_t *state = *state_store;

  game_remove_meeple(&engine->game, state->x, state->y, state->group);

  return SUCCESS;
}

return_code_t meeple_place_free(void **state_store, engine_t *engine) {
  meeple_place_hook_state_t *state = *state_store;

  free(state);

  return SUCCESS;
}

return_code_t meeple_place_list_actions(action_vector_t *actions,
                                        engine_t        *engine) {
  player_t *player = game_get_current_player(&engine->game);

  actions = malloc(sizeof(action_vector_t));

  action_t action_none = {.order = {0}, .type = LIBCARCASSONNE_ACTION_NONE};

  if (!player_has_meeple_to_place(player)) {
    // If player does not have meeple left
    // We return only action none
    vector_alloc(actions, 1);

    vector_append(actions, &action_none);

    return SUCCESS;
  }

  // Looking for the tile placed this turn
  size_t      index    = vector_size(&engine->dispatchs) - 1;
  dispatch_t *dispatch = vector_nth(&engine->dispatchs, index);

  while (dispatch->action->type != LIBCARCASSONNE_ACTION_PLACE_TILE) {
    index--;
    dispatch = vector_nth(&engine->dispatchs, index);
    if (dispatch->action->type == LIBCARCASSONNE_ACTION_END_PLAYER_TURN) {
      return INVALID_ACTION;
    }
  }

  placed_tile_t **tile =
      game_tile_at(&engine->game, dispatch->action->order.place_tile.x,
                   dispatch->action->order.place_tile.y);

  if (tile == NULL || *tile == NULL) {
    return NO_TILE;
  }

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

  game_remove_tile(&engine->game, state->x, state->y);

  return SUCCESS;
}

return_code_t tile_place_free(void **state_store, engine_t *engine) {
  tile_place_hook_state_t *state = *state_store;

  free(state);

  return SUCCESS;
}

return_code_t tile_place_list_actions(action_vector_t *actions,
                                      engine_t        *engine) {
  const tile_t *tile = deck_pick(&engine->game.deck);

  vector_alloc(actions, 5);

  vector2d_vector_t vec = game_get_available_space(&engine->game);

  for (unsigned int i = 0; i < vector_size(&vec); i++) {
    vector2d_t spot = *vector_nth(&vec, i);

    for (tile_orientation_t orientation = 0; i < 4; i++) {
      if (game_is_tile_placeable(&engine->game, tile, spot.x, spot.y,
                                 orientation)) {
        action_t action = {.type  = LIBCARCASSONNE_ACTION_PLACE_TILE,
                           .order = {.place_tile = {.orientation = orientation,
                                                    .x           = spot.x,
                                                    .y           = spot.y,
                                                    .tile        = tile}}};
        vector_append(actions, &action);
      }
    }
  }

  vector_free(&vec);

  return SUCCESS;
}

static void update_score(engine_t                        *engine,
                         placed_tile_group_eval_points_t *evaluation) {
  int players_meeple_count[LIBCARCASSONNE_MAX_PLAYERS] = {0};
  int winner_score                                     = 0;

  for (size_t i = 0; i < LIBCARCASSONNE_MAX_PLAYERS; i++) {
    for (size_t j = 0; j < vector_size(&evaluation->meeples); j++) {
      meeple_t *meeple = *vector_nth(&evaluation->meeples, j);

      // Identifying meeple's player
      if (&engine->game.players[i] == meeple->player) {
        players_meeple_count[i]++;

        const placed_tile_t *meeple_tile = meeple->group_node->tile;

        // Giving back meeple to player
        game_remove_meeple(&engine->game, meeple_tile->x, meeple_tile->y,
                           meeple->group);

        // Computing max
        if (winner_score < players_meeple_count[i]) {
          winner_score = players_meeple_count[i];
        }
      }
    }
  }

  for (size_t i = 0; i < LIBCARCASSONNE_MAX_PLAYERS; i++) {
    if (players_meeple_count[i] == winner_score) {
      engine->game.players[i].score += evaluation->points;
    }
  }
}

return_code_t give_back_meeples_fw(void **state_store, engine_t *engine,
                                   action_t *action) {
  // on récupère la dernière action "place"
  tile_place_hook_state_t *place = NULL;
  for (unsigned j = vector_size(&engine->dispatchs) - 1; j != 0; j--) {
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

  placed_tile_group_eval_points_vector_t evals;
  vector_alloc(&evals, 9);
  evals.meta.size = 0;

  *state_store = &evals;

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!visite[group]) {
      placed_tile_group_t *groupp = placed_tile->groups[group];

      if (placed_tile_group_complete(groupp)) {
        placed_tile_group_eval_points_t evaluation =
            placed_tile_group_eval_points(groupp);

        placed_tile_group_eval_points_t *stored_eval =
            malloc(sizeof(placed_tile_group_eval_points_t));
        memcpy(stored_eval, &evaluation,
               sizeof(placed_tile_group_eval_points_t));

        vector_append(&evals, stored_eval);

        update_score(engine, &evaluation);
      }
    }
    visite[group] = true;
  }

  return SUCCESS;
}

static void revert_update_score(engine_t                        *engine,
                                placed_tile_group_eval_points_t *evaluation) {
  int players_meeple_count[LIBCARCASSONNE_MAX_PLAYERS] = {0};
  int winner_score                                     = 0;

  for (size_t i = 0; i < LIBCARCASSONNE_MAX_PLAYERS; i++) {
    for (size_t j = 0; j < vector_size(&evaluation->meeples); j++) {
      meeple_t *meeple = *vector_nth(&evaluation->meeples, j);

      // Identifying meeple's player
      if (&engine->game.players[i] == meeple->player) {
        players_meeple_count[i]++;

        placed_tile_t *meeple_tile = meeple->group_node->tile;
        game_place_meeple(&engine->game, meeple_tile->x, meeple_tile->y,
                          meeple->group, meeple->meeple_type, meeple->player);

        // Giving back meeple to player
        (*vector_nth(&engine->game.players[i].meeples_count,
                     meeple->meeple_type))
            .count--;

        // Computing max
        if (winner_score < players_meeple_count[i]) {
          winner_score = players_meeple_count[i];
        }
      }
    }
  }

  for (size_t i = 0; i < LIBCARCASSONNE_MAX_PLAYERS; i++) {
    if (players_meeple_count[i] == winner_score) {
      engine->game.players[i].score -= evaluation->points;
    }
  }
}

return_code_t give_back_meeples_bw(void **state_store, engine_t *engine) {
  // annulation des meeples rendus

  placed_tile_group_eval_points_vector_t *evals = (*state_store);
  if (evals == NULL) {
    return SUCCESS;
  }

  for (size_t i = 0; i < vector_size(evals); i++) {
    placed_tile_group_eval_points_t *eval = vector_nth(evals, i);
    revert_update_score(engine, eval);
  }

  return SUCCESS;
}

return_code_t give_back_meeples_free(void **state_store, engine_t *engine) {
  placed_tile_group_eval_points_vector_t *evals = (*state_store);
  if (evals == NULL) {
    return SUCCESS;
  }

  for (size_t i = 0; i < vector_size(evals); i++) {
    placed_tile_group_eval_points_t *eval = vector_nth(evals, i);
    free(eval);
  }

  free(*state_store);

  return SUCCESS;
}

return_code_t give_back_meeples_list_actions(action_vector_t *actions,
                                             engine_t        *engine) {
  vector_alloc(actions, 1);

  action_t action = {0};
  action.type     = LIBCARCASSONNE_ACTION_NONE;

  vector_append(actions, &action);

  return SUCCESS;
}

return_code_t next_player_fw(void **state_store, engine_t *engine,
                             action_t *action) {
  (void)state_store;
  (void)action;

  engine->game.current_player =
      (engine->game.current_player + 1) % engine->config.players;

  return SUCCESS;
}
return_code_t next_player_bw(void **state_store, engine_t *engine) {
  (void)state_store;

  if (vector_size(&engine->dispatchs) == 0) {
    return ERROR;
  }

  if (engine->game.current_player == 0) {
    engine->game.current_player = engine->config.players;
  }

  engine->game.current_player--;

  return SUCCESS;
}

return_code_t next_player_free(void **state_store, engine_t *engine) {
  return SUCCESS;
}

return_code_t next_player_list_actions(action_vector_t *actions,
                                       engine_t        *engine) {
  actions = calloc(1, sizeof(action_vector_t));

  vector_alloc(actions, 1);

  action_t action = {0};
  action.type     = LIBCARCASSONNE_ACTION_NONE;

  vector_append(actions, &action);

  return SUCCESS;
}
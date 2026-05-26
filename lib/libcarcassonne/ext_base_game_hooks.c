#include <assert.h>
#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/game.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
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
#include "libutils/hashmap.h"
#include "libutils/lc.h"
#include "libutils/vector.h"

return_code_t meeple_place_fw(void **state_store, engine_t *engine,
                              action_t *action) {
  *state_store                     = malloc(sizeof(meeple_place_hook_state_t));
  meeple_place_hook_state_t *state = *state_store;

  if (action->order.place_meeple.meeple_type == NONE) {
    return SUCCESS;
  }

  state->x           = action->order.place_meeple.x;
  state->y           = action->order.place_meeple.y;
  state->group       = action->order.place_meeple.part_group;
  state->meeple_type = action->order.place_meeple.meeple_type;

  // on place le meeple
  return game_place_meeple(&engine->game, state->x, state->y, state->group,
                           state->meeple_type,
                           game_get_current_player(&engine->game));
}

return_code_t meeple_place_bw(void **state_store, engine_t *engine) {
  meeple_place_hook_state_t *state = *state_store;

  if (state->meeple_type == NONE) {
    return SUCCESS;
  }

  return game_remove_meeple(&engine->game, state->x, state->y, state->group);
}

return_code_t meeple_place_free(void **state_store, engine_t *engine) {
  meeple_place_hook_state_t *state = *state_store;

  free(state);

  return SUCCESS;
}

static dispatch_t *find_last_place_tile_dispatch(engine_t *engine) {
  int index = vector_size(&engine->dispatchs) - 1;

  while (index >= 0) {
    dispatch_t *dispatch = vector_nth(&engine->dispatchs, index);

    if (dispatch->hook == &hook_next_player) {
      return NULL;
    }

    if (dispatch->action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
      return dispatch;
    }

    index--;
  }

  return NULL;
}

return_code_t meeple_place_list_actions(action_vector_t *actions,
                                        engine_t        *engine) {
  player_t *player = game_get_current_player(&engine->game);

  action_t action_none = {.order.place_meeple = {0},
                          .type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE};
  action_none.order.place_meeple.meeple_type = NONE;

  if (!player_has_meeple_to_place(player)) {
    // If player does not have meeple left
    // We return only action none
    vector_alloc(actions, 1);

    vector_append(actions, &action_none);

    return SUCCESS;
  }

  dispatch_t *dispatch = find_last_place_tile_dispatch(engine);

  if (!dispatch) {
    return INVALID_ACTION;
  }

  placed_tile_t **tile =
      game_tile_at(&engine->game, dispatch->action->order.place_tile.x,
                   dispatch->action->order.place_tile.y);

  if (tile == NULL || *tile == NULL) {
    return NO_TILE;
  }

  bool visited[9] = {0};

  vector_alloc(actions, 4);

  for (unsigned int i = 0; i < PLACED_TILE_GROUP_NUMBER; i++) {
    placed_tile_group_t *group = (*tile)->groups[i];

    tile_part_group_t id = (*tile)->parent->parts_groups[i];
    if (group != NULL && !visited[id]) {
      visited[id] = true;
      placed_tile_group_eval_points_t eval =
          placed_tile_group_eval_points(group, false);

      if (vector_size(&eval.meeples) == 0) {
        for (meeple_type_t meeple_type = BASIC; meeple_type < 3;
             meeple_type++) {
          meeple_count_t count =
              *vector_nth(&player->meeples_count, meeple_type);
          if (count.count > 0) {
            action_t action = {
                .type               = LIBCARCASSONNE_ACTION_PLACE_MEEPLE,
                .order.place_meeple = {.part_group  = id,
                                       .x           = (*tile)->x,
                                       .y           = (*tile)->y,
                                       .tile        = (*tile),
                                       .meeple_type = meeple_type}};
            vector_append(actions, &action);
          }
        }
      }

      vector_free(&eval.meeples);
    }
  }

  vector_append(actions, &action_none);

  return SUCCESS;
}

return_code_t tile_place_fw(void **state_store, engine_t *engine,
                            action_t *action) {
  *state_store                   = malloc(sizeof(tile_place_hook_state_t));
  tile_place_hook_state_t *state = *state_store;

  state->x = action->order.place_tile.x;
  state->y = action->order.place_tile.y;

  // on place la tile
  return game_place_tile(&engine->game, action->order.place_tile.tile, state->x,
                         state->y, action->order.place_tile.orientation);
}

return_code_t tile_place_bw(void **state_store, engine_t *engine) {
  tile_place_hook_state_t *state = *state_store;

  return game_remove_tile(&engine->game, state->x, state->y);
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
  unsigned int player_score[LIBCARCASSONNE_MAX_PLAYERS] = {0};
  unsigned int winner_nb_meeple                         = 0;

  if (vector_size(&evaluation->meeples) == 0) {
    return;
  }

  for (unsigned int i = 0; i < vector_size(&evaluation->meeples); i++) {
    meeple_t *meeple = *vector_nth(&evaluation->meeples, i);
    player_score[meeple->player->id]++;

    const placed_tile_t *meeple_tile = meeple->group_node->tile;

    if (winner_nb_meeple < player_score[meeple->player->id]) {
      winner_nb_meeple = player_score[meeple->player->id];
    }

    // Giving back meeple to player
    assert(game_remove_meeple(&engine->game, meeple_tile->x, meeple_tile->y,
                              meeple->group) == SUCCESS);
  }

  for (size_t i = 0; i < engine->config.players; i++) {
    unsigned int meeple_nb = player_score[i];
    if (meeple_nb == winner_nb_meeple) {
      engine->game.players[i].score += evaluation->points;
    }
  }
}

return_code_t give_back_meeples_fw(void **state_store, engine_t *engine,
                                   action_t *action) {
  // on récupère la dernière action "place"
  dispatch_t *dispatch = find_last_place_tile_dispatch(engine);

  if (dispatch == NULL) {
    return NULL_POINTER;
  }

  action_t *place = dispatch->action;

  placed_tile_t **tile = game_tile_at(&engine->game, place->order.place_tile.x,
                                      place->order.place_tile.y);
  if (tile == NULL || *tile == NULL) {
    return NULL_POINTER;
  }

  placed_tile_t *placed_tile = *tile;

  // pour chacun des groupes de la tile placée, si il y a des meeples, on les
  // retire
  bool visite[9];
  memset(&visite, 0, sizeof(visite));

  placed_tile_group_eval_points_vector_t *evals =
      calloc(1, sizeof(placed_tile_group_eval_points_vector_t));
  vector_alloc(evals, 9);
  evals->meta.size = 0;

  *state_store = evals;

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!visite[group]) {
      placed_tile_group_t *groupp = placed_tile->groups[group];

      if (placed_tile_group_complete(groupp)) {
        placed_tile_group_eval_points_t evaluation =
            placed_tile_group_eval_points(groupp, true);

        vector_append(evals, &evaluation);

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
        return_code_t  code        = game_place_meeple(
            &engine->game, meeple_tile->x, meeple_tile->y, meeple->group,
            meeple->meeple_type, meeple->player);

        assert(code == SUCCESS);

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

  for (unsigned int i=0; i<vector_size(evals); i++) {
    placed_tile_group_eval_points_t *eval = vector_nth(evals, i);
    vector_free(&eval->meeples);
  }
  vector_free(evals);

  free(evals);

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

  return_code_t code = game_end_player_turn(&engine->game);

  if (code == NO_MORE_PLAYER) {
    return game_end_round(&engine->game);
  }

  return code;
}
return_code_t next_player_bw(void **state_store, engine_t *engine) {
  (void)state_store;

  if (vector_size(&engine->dispatchs) == 0) {
    return ERROR;
  }

  // Si on était au premier joueur avant le bw, c'est qu'on avait fini un tour
  // Donc il faut décrémenter game.turn
  if (engine->game.current_player == 0) {
    engine->game.turn--;
    engine->game.current_player = engine->config.players - 1;
  } else {
    engine->game.current_player--;
  }

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

/// @brief Calcule les points des constructions inachevées, retire les meeples
/// @param game La partie
/// @param removed_meeples Vecteur où stocker les infos des meeples retirés
static void compute_unfinished_points(
    engine_t *engine, end_game_removed_meeples_vector_t *removed_meeples) {
      
  for (unsigned int p = 0; p < engine->game.options->players; p++) {
    player_t *player = &engine->game.players[p];

    for (size_t m = 0; m < vector_size(&player->meeples); m++) {
      meeple_t            *meeple = *vector_nth(&player->meeples, m);
      placed_tile_group_t *group  = meeple->group_node;

      // Seuls les groupes INACHEVÉS comptent
      if (placed_tile_group_complete(group)) {
        continue;
      }

      // Stocker les infos du meeple AVANT de le retirer
      removed_meeple_t removed = {.x      = group->tile->x,
                                  .y      = group->tile->y,
                                  .group  = meeple->group,
                                  .type   = meeple->meeple_type,
                                  .player = meeple->player};
      vector_append(removed_meeples, &removed);
    }
  }
  for (unsigned int p = 0; p < engine->game.options->players; p++) {
    player_t *player = &engine->game.players[p];

    while (vector_size(&player->meeples)>0) {
      meeple_t *meeple=*vector_nth(&player->meeples, 0);
      placed_tile_group_eval_points_t evaluation = placed_tile_group_eval_points(meeple->group_node, false);
      update_score(engine, &evaluation);
      vector_free(&evaluation.meeples);
    }
  }
}

return_code_t end_game_fw(void **state_store, engine_t *engine,
                          action_t *action) {
  (void)action;

  if (!is_game_finished(&engine->game)) {
    return SUCCESS;  // Pas encore la fin
  }

  // Allouer et initialiser le state
  *state_store                 = calloc(1, sizeof(end_game_hook_state_t));
  end_game_hook_state_t *state = *state_store;

  // Sauvegarder les scores actuels
  for (unsigned int i = 0; i < engine->config.players; i++) {
    state->saved_scores[i] = engine->game.players[i].score;
  }

  // Initialiser la liste des meeples retirés
  vector_alloc(&state->removed_meeples, 10);

  // Retirer les meeples des groupes inachevés (préparation pour le calcul)
  compute_unfinished_points(engine, &state->removed_meeples);

  engine->game.state = GAME_STATE_FINISHED;
  return SUCCESS;
}

return_code_t end_game_bw(void **state_store, engine_t *engine) {
  if (*state_store == NULL) {
    return SUCCESS;
  }

  end_game_hook_state_t *state = *state_store;

  // Restaurer les scores
  for (unsigned int i = 0; i < engine->config.players; i++) {
    engine->game.players[i].score = state->saved_scores[i];
  }

  // Restaurer les meeples retirés
  for (size_t i = 0; i < vector_size(&state->removed_meeples); i++) {
    removed_meeple_t *removed = vector_nth(&state->removed_meeples, i);
    game_place_meeple(&engine->game, removed->x, removed->y, removed->group,
                      removed->type, removed->player);
  }

  engine->game.state = GAME_STATE_PLAYING;
  return SUCCESS;
}

return_code_t end_game_free(void **state_store, engine_t *engine) {
  (void)engine;
  if (*state_store == NULL) {
    return SUCCESS;
  }
  end_game_hook_state_t *state = *state_store;
  vector_free(&state->removed_meeples);
  free(state);
  return SUCCESS;
}

return_code_t end_game_list_actions(action_vector_t *actions,
                                    engine_t        *engine) {
  (void)engine;
  vector_alloc(actions, 1);
  action_t action = {.type = LIBCARCASSONNE_ACTION_NONE};
  vector_append(actions, &action);
  return SUCCESS;
}
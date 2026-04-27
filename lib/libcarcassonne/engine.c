#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/game.h>
#include <unistd.h>

#include "libcarcassonne/engine_state.h"
#include "libcarcassonne/forward.h"
#include "libutils/lc.h"
#include "libutils/vector.h"

return_code_t create_engine(engine_t *engine, options_t options) {
  if (engine == NULL) {
    return ERROR;
  }

  engine->state = LIBCARCASSONNE_ENGINE_WAITING_GAME_START;

  engine->config = options;

  extension_process_hooks_list_t hooks;

  for (unsigned int i = 0; i < vector_size(&options.extensions); i++) {
    extension_t *ext = *vector_nth(&options.extensions, i);
    for (unsigned int j = 0; j < ext->hooks->meta.size; j++) {
      const extension_process_hook_t *hook = *vector_nth(ext->hooks, j);
      unsigned int                    k    = 0;
      for (; k < vector_size(&hooks); k++) {
        if ((*list_value(&hooks, list_nth(&hooks, k)))->priority <
            hook->priority) {
          k++;
        }
        list_insert(&hooks, &hook, k);
      }
    }
  }

  engine->current_hook = 0;

  return_code_t code = create_game(&engine->game, &engine->config);
  if (code != SUCCESS) {
    return code;
  }

  engine->state = LIBCARCASSONNE_ENGINE_WAITING_GAME_START;

  return SUCCESS;
}

return_code_t destroy_engine(engine_t *engine) {
  if (engine == NULL) {
    return NULL_POINTER;
  }

  vector_free(&engine->hooks);

  destroy_game(&engine->game);

  return SUCCESS;
}

return_code_t start_game(engine_t *engine) {
  if (engine == NULL) return NULL_POINTER;

  return_code_t code =
      game_place_tile(&engine->game, deck_pick(&engine->game.deck), 0, 0,
                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  if (code != SUCCESS) {
    return code;
  }

  engine->state = LIBCARCASSONNE_ENGINE_NULL_ENGINE;

  return SUCCESS;
}

/// @brief Gère le placement d'une tuile
/// @param engine Le moteur auquel on demande de placer une tuile
/// @param action L'action à effectuer
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_place_tile(engine_t *engine, action_t action) {
  if (engine->state != LIBCARCASSONNE_ENGINE_WAITING_PLAYER_TILE_ACTION)
    return INVALID_ACTION;

  game_t *game = &engine->game;

  tile_t            *tile        = action.order.place_tile.tile;
  int                x           = action.order.place_tile.x;
  int                y           = action.order.place_tile.y;
  tile_orientation_t orientation = action.order.place_tile.orientation;

  return_code_t code = game_place_tile(game, tile, x, y, orientation);

  if (code == SUCCESS) {
    engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION;
  }
  return code;
}

/// @brief Gère le placement d'un meeple
/// @param engine Le moteur auquel on demande de placer une tuile
/// @param action L'action à effectuer
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_place_meeple(engine_t *engine, action_t action) {
  if (engine->state != LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION)
    return INVALID_ACTION;

  game_t *game = &engine->game;

  int           part_group = action.order.place_meeple.part_group;
  int           x          = action.order.place_meeple.x;
  int           y          = action.order.place_meeple.y;
  meeple_type_t type       = action.order.place_meeple.meeple_type;

  return game_place_meeple(game, x, y, part_group, type);
}

/// @brief Gère la fin du tour d'un joueur
/// @param engine Le moteur auquel on demande de placer une tuile
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_end_player_turn(engine_t *engine) {
  if (engine->state != LIBCARCASSONNE_ENGINE_WAITING_PLAYER_END_TURN)
    return INVALID_ACTION;

  return_code_t code = game_end_player_turn(&engine->game);
  if (code == NO_MORE_PLAYER)  // Si on a plus de joueur à faire jouer on passe
                               // au round suivant
    return game_end_round(&engine->game);
  else  // Sinon, on renvoie le code de succès ou d'erreur
    return code;
}

return_code_t dispatch_action(engine_t *engine, action_t action) {
  do {
    const extension_process_hook_t *current_hook =
        (*vector_nth(&engine->hooks, engine->current_hook));

    if (action.type != current_hook->needed_action) {
      return NO_PROGRESS;
    }

    dispatch_t dispatch;
    vector_append(&engine->dispatchs, &dispatch);

    dispatch_t *store =
        vector_nth(&engine->dispatchs, vector_size(&engine->dispatchs) - 1);
    current_hook->fw(&(store->state_store), engine, &action);

    engine->current_hook =
        (engine->current_hook + 1) % vector_size(&engine->hooks);

  } while (engine->current_hook != 0);

  return SUCCESS;
}

engine_state_t get_engine_state(engine_t *engine) {
  if (engine == NULL) return LIBCARCASSONNE_ENGINE_NULL_ENGINE;
  return engine->state;
}

return_code_t engine_adapt_state(engine_t *engine) {
  switch ((*vector_nth(&engine->hooks, engine->current_hook))->needed_action) {
    case LIBCARCASSONNE_ACTION_PLACE_TILE:
      engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_TILE_ACTION;
      break;
    case LIBCARCASSONNE_ACTION_PLACE_MEEPLE:
      engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION;
      break;
    case LIBCARCASSONNE_ACTION_END_PLAYER_TURN:
      engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_END_TURN;
      break;
    default:
      engine->state = LIBCARCASSONNE_ENGINE_NULL_ENGINE;
  }

  return SUCCESS;
}

return_code_t engine_revert(engine_t *engine, unsigned int epoch) {
  unsigned int i = vector_size(&engine->dispatchs);

  do {
    const extension_process_hook_t *current_hook =
        (*vector_nth(&engine->hooks, engine->current_hook));

    dispatch_t *store =
        vector_nth(&engine->dispatchs, vector_size(&engine->dispatchs) - 1);
    current_hook->bw(&store->state_store, engine);

    current_hook->free(&store->state_store);

    vector_remove(&engine->dispatchs, vector_size(&engine->hooks));

    if (engine->current_hook == 0) {
      engine->current_hook = vector_size(&engine->hooks);
    } else {
      engine->current_hook = (engine->current_hook - 1);
    }

    i++;
  } while (i == epoch);

  return SUCCESS;
}
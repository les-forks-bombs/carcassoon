#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/game.h>
#include <unistd.h>

#include "libcarcassonne/dispatch.h"
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
      for (unsigned int k = 0; k < vector_size(&hooks); k++) {
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
  if (engine == NULL) {
    return NULL_POINTER;
  }

  return_code_t code =
      game_place_tile(&engine->game, deck_pick(&engine->game.deck), 0, 0,
                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  if (code != SUCCESS) {
    return code;
  }

  engine->state = LIBCARCASSONNE_ENGINE_NULL_ENGINE;

  return SUCCESS;
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

    store->action      = &action;
    store->hook        = current_hook;
    store->state_store = NULL;

    current_hook->fw(&(store->state_store), engine, &action);

    engine->current_hook =
        (engine->current_hook + 1) % vector_size(&engine->hooks);

  } while (engine->current_hook != 0);

  engine->last_tour_epoch = vector_size(&engine->dispatchs);

  return SUCCESS;
}

engine_state_t get_engine_state(engine_t *engine) {
  if (engine == NULL) {
    return LIBCARCASSONNE_ENGINE_NULL_ENGINE;
  }
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

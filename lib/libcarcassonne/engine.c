#include "libcarcassonne/engine.h"

#include <libcarcassonne/libcarcassonne.h>
#include <libutils/lc.h>
#include <libutils/vector.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forward.h"
#include "libcarcassonne/enums.h"
#include "libcarcassonne/game.h"

return_code_t create_engine(engine_t *engine, options_t options) {
  if (engine == NULL) {
    return ERROR;
  }

  engine->config = options;

  extension_process_hooks_list_t hooks = {0};

  for (unsigned int i = 0; i < vector_size(&options.extensions); i++) {
    extension_t *ext = *vector_nth(&options.extensions, i);
    for (unsigned int j = 0; j < vector_size(ext->hooks); j++) {
      const extension_process_hook_t *hook = *vector_nth(ext->hooks, j);

      unsigned int k;
      for (k = 0; k < vector_size(&hooks); k++) {
        if ((*list_value(&hooks, list_nth(&hooks, k)))->priority >=
            hook->priority) {
          break;
        }
      }
      list_insert(&hooks, &hook, k);
    }
  }

  for (size_t i = 0; i < list_size(&hooks); i++) {
    vector_append(
        &engine->hooks,
        ((const extension_process_hook_t **)(&list_nth(&hooks, i)->value)));
  }

  list_free(&hooks);
  engine->current_hook = 0;

  return_code_t code = create_game(&engine->game, &engine->config);
  if (code != SUCCESS) {
    return code;
  }

  return SUCCESS;
}

return_code_t destroy_engine(engine_t *engine) {
  if (engine == NULL) {
    return NULL_POINTER;
  }

  vector_free(&engine->hooks);

  for (size_t i = 0; i < vector_size(&engine->dispatchs); i++) {
    dispatch_t *dispatch = vector_nth(&engine->dispatchs, i);
    dispatch->hook->free(&dispatch->state_store, engine);
    free(dispatch->action);
  }

  vector_free(&engine->dispatchs);

  destroy_game(&engine->game);

  return SUCCESS;
}

return_code_t start_game(engine_t *engine) {
  if (engine == NULL) {
    return NULL_POINTER;
  }
  if (engine->game.state != GAME_STATE_NOT_STARTED) {
    return GAME_NOT_STARTED;
  }

  return_code_t code =
      game_place_tile(&engine->game, deck_pick(&engine->game.deck), 0, 0,
                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  if (code != SUCCESS) {
    return code;
  }

  engine->game.state = GAME_STATE_PLAYING;

  action_t action_none = {0};
  code                 = dispatch_action(engine, action_none);

  if (code == NO_PROGRESS) {
    return SUCCESS;
  }

  return code;
}

return_code_t dispatch_action(engine_t *engine, action_t action) {
  if (engine->game.state == GAME_STATE_NOT_STARTED) {
    return GAME_NOT_STARTED;
  }
  if (engine->game.state == GAME_STATE_FINISHED) {
    return GAME_FINISHED;
  }

  return_code_t code = NO_PROGRESS;

  action_t action_none = {0};

  while (true) {
    const extension_process_hook_t *current_hook =
        (*vector_nth(&engine->hooks, engine->current_hook));

    if ((action.type != current_hook->needed_action &&
         current_hook->needed_action != LIBCARCASSONNE_ACTION_NONE)) {
      return code;
    }

    dispatch_t dispatch = {0};
    vector_append(&engine->dispatchs, &dispatch);

    dispatch_t *store =
        vector_nth(&engine->dispatchs, vector_size(&engine->dispatchs) - 1);

    action_none.type = LIBCARCASSONNE_ACTION_NONE;
    store->action    = calloc(1, sizeof(action_t));
    memcpy(store->action, &action, sizeof(action_t));
    store->hook        = current_hook;
    store->state_store = NULL;

#ifdef DEBUG
    // printf("Exécution du hook: %s\n", current_hook->label);
#endif

    return_code_t error_code =
        current_hook->fw(&(store->state_store), engine, &action);
    if (error_code != SUCCESS) {
      return error_code;
    }

    engine->current_hook =
        (engine->current_hook + 1) % vector_size(&engine->hooks);

#ifdef DEBUG
    // printf("Prochain du hook:
    // %s\n",(*vector_nth(&engine->hooks,engine->current_hook))->label);
#endif

    if (engine->current_hook == vector_size(&engine->hooks) - 1) {
      code = SUCCESS;
    }

    if (engine->game.state == GAME_STATE_FINISHED) {
      return code;
    }
  }
}

return_code_t engine_revert(engine_t *engine, unsigned int epoch) {
  if (vector_size(&engine->dispatchs) <= epoch) {
    return ERROR;
  }

  unsigned int i = vector_size(&engine->dispatchs);

  do {
    dispatch_t *store =
        vector_nth(&engine->dispatchs, vector_size(&engine->dispatchs) - 1);

    const extension_process_hook_t *current_hook = store->hook;

    current_hook->bw(&store->state_store, engine);
    current_hook->free(&store->state_store, engine);

    free(store->action);

    vector_remove(&engine->dispatchs, vector_size(&engine->dispatchs) - 1);

    i--;
  } while (i != epoch);

  engine->current_hook =
      vector_size(&engine->dispatchs) % vector_size(&engine->hooks);

  return SUCCESS;
}

action_type_t engine_wanted_action(engine_t *engine) {
  return (*vector_nth(&engine->hooks, engine->current_hook))->needed_action;
}

action_vector_t engine_get_actions(engine_t *engine) {
  action_vector_t vec = {0};

  const extension_process_hook_t *current_hook =
      *vector_nth(&engine->hooks, engine->current_hook);

  current_hook->list_actions(&vec, engine);

  return vec;
}

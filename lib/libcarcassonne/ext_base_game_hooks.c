#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/game.h>
#include <stdlib.h>

return_code_t meeple_place_fw(void** state_store, engine_t* engine,
                              action_t* action) {
  if (action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE) {
    *state_store = malloc(sizeof(meeple_place_hook_state_t));
    meeple_place_hook_state_t* state = *state_store;

    state->x     = action->order.place_meeple.x;
    state->y     = action->order.place_meeple.y;
    state->group = action->order.place_meeple.part_group;

    // on place le meeple
    game_place_meeple(&engine->game, state->x, state->y, state->group,
                      action->order.place_meeple.meeple_type);
    engine->game.players[engine->game.current_player].nb_meeples--;

    return SUCCESS;
  } else {
    engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION;
    return NO_PROGRESS;
  }
}

return_code_t meeple_place_bw(void** state_store, engine_t* engine) {
  meeple_place_hook_state_t* state = *state_store;

  // game_remove_meeple(&engine->game, state->x, state->y, state->group);
  engine->game.players[engine->game.current_player].nb_meeples++;

  return SUCCESS;
}

return_code_t meeple_place_state_free(void* state_store) { free(state_store); }

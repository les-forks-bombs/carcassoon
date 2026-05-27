#include <sdl/action.h>
#include <stdio.h>

#include "libutils/vector.h"
#include "sdl/map.h"

void get_current_actions(AppState *as) {
  action_vector_t actions     = engine_get_actions(&as->engine);
  action_t       *action_none = NULL;

  if (actions.meta.size == 0) {
    printf("Aucune action possible\n");
    return;
  }
  vector_free(&as->all_actions);

  for (int i = 0; i < vector_size(&actions); i++) {
    action_t *a = vector_nth(&actions, i);
    if (a->type == LIBCARCASSONNE_ACTION_PLACE_TILE ||
        a->order.place_meeple.tile != NULL) {
      vector_append(&as->all_actions, a);
    } else if (a->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE &&
               a->order.place_meeple.tile == NULL) {
      printf("Meeple none ajouté\n");
      action_none = a;
    }
  }
  if (vector_size(&as->all_actions) == 0 && action_none != NULL) {
    // printf("Pas de placement de meeple possible, passer au placement de
    // tile\n");
    dispatch_action(&as->engine, *action_none);
    get_current_actions(as);
    put_first_action_in_appstate(as);
    // printf("prochain hook: %s\n",
    // (*vector_nth(&as->engine.hooks,as->engine.current_hook))->label);
    // printf("current_action: %d\n", as->current_action==NULL);
    bool next_action_is_tile_placement =
        as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_TILE;
    if (next_action_is_tile_placement) {
      update_possible_places(as);
    } else {
      update_possible_meeples(as);
    }
  }
}

void put_first_action_in_appstate(AppState *as) {
  as->current_action      = vector_nth(&as->all_actions, 0);
  as->is_waiting_for_tile = true;
  as->action_index        = 0;
}

void pass_to_action(AppState *as, int increment) {
  size_t total_actions = vector_size(&as->all_actions);

  if (total_actions == 0) {
    return;
  }
  int next_index = (int)as->action_index + increment;

  if (next_index >= (int)total_actions) {
    put_first_action_in_appstate(as);
  } else if (next_index < 0) {
    as->action_index   = (int)total_actions - 1;
    as->current_action = vector_nth(&as->all_actions, as->action_index);
  } else {
    as->action_index   = (size_t)next_index;
    as->current_action = vector_nth(&as->all_actions, as->action_index);
  }
  // printf("action changée\n");
}

void send_action_to_engine(AppState *as) {
  printf("hook actuel: %s\n",
         (*vector_nth(&as->engine.hooks, as->engine.current_hook))->label);
  dispatch_action(&as->engine, *as->current_action);
  get_current_actions(as);
  put_first_action_in_appstate(as);
  printf("prochain hook: %s\n",
         (*vector_nth(&as->engine.hooks, as->engine.current_hook))->label);
  printf("current_action: %d\n", as->current_action == NULL);
  bool next_action_is_tile_placement =
      as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_TILE;
  if (next_action_is_tile_placement) {
    update_possible_places(as);
  } else {
    update_possible_meeples(as);
  }
}
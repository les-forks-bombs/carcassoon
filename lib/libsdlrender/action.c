#include <assert.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/action.h>
#include <libsdlrender/banner.h>
#include <libsdlrender/forward.h>
#include <libsdlrender/map.h>
#include <libsdlrender/meeple.h>
#include <libutils/vector.h>
#include <stdio.h>

void get_current_actions(appstate_t *as) {
  vector_free(&as->all_actions);
  as->all_actions = engine_get_actions(as->engine);

  if (as->all_actions.meta.size == 0) {
    printf("Aucune action possible\n");
    return;
  }

  put_first_action_in_appstate(as);
  printf("prochain hook: %s\n",
         (*vector_nth(&as->engine->hooks, as->engine->current_hook))->label);
  printf("current_action: %d\n", as->current_action == NULL);
  bool next_action_is_tile_placement =
      as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_TILE;
  if (next_action_is_tile_placement) {
    update_possible_places(as);
  } else {
    update_possible_meeples(as);
  }

  if (vector_size(&as->all_actions) == 1 &&
      as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE) {
    printf("Pas possible de placer un meeple, on dispatch\n");
    send_action_to_engine(as);
  }
  printf("###### nombre d'actions possibles: %d\n",
         vector_size(&as->all_actions));
  for (unsigned int i = 0; i < vector_size(&as->all_actions); i++) {
    action_t *action = vector_nth(&as->all_actions, i);
    if (action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE) {
      printf("[ACTION MEEPLE %d] type = %d, groupe = %d\n", i,
             action->order.place_meeple.meeple_type,
             action->order.place_meeple.part_group);
    } else if (action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
      printf("[ACTION TILE %d] x = %d, y = %d, rotation = %d\n", i,
             action->order.place_tile.x, action->order.place_tile.y,
             action->order.place_tile.orientation);
    }
  }
}

void put_first_action_in_appstate(appstate_t *as) {
  as->current_action      = vector_nth(&as->all_actions, 0);
  as->is_waiting_for_tile = true;
  as->action_index        = 0;
}

void pass_to_action(appstate_t *as, int increment) {
  size_t total_actions = vector_size(&as->all_actions);

  if (total_actions == 0) {
    return;
  }
  int next_index = as->action_index + increment;

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

void send_action_to_engine(appstate_t *as) {
  // printf("hook actuel: %s\n", (*vector_nth(&as->engine.hooks,
  // as->engine.current_hook))->label);
  return_code_t result = dispatch_action(as->engine, *as->current_action);
  assert(result == SUCCESS || result == NO_PROGRESS);
  get_current_actions(as);
  synchronize_banners(as);
}

#include <sdl/action.h>
#include "libutils/vector.h"
#include <stdio.h>

void get_current_actions(AppState *as){
    action_vector_t actions = engine_get_actions(&as->engine);

    vector_free(&as->all_actions);

    for (int i = 0; i < vector_size(&actions); i++) {
        action_t *a = vector_nth(&actions, i);
        if (a->type == LIBCARCASSONNE_ACTION_PLACE_TILE || a->order.place_meeple.tile!=NULL) {
            vector_append(&as->all_actions, a);
        }
    }
}

void put_first_action_in_appstate(AppState *as) {
    as->current_action=vector_nth(&as->all_actions, 0);
    as->is_waiting_for_tile=true;
    as->action_index=0;
}

void pass_to_action(AppState *as, int increment) {
    size_t total_actions = vector_size(&as->all_actions);

    if (total_actions == 0) {
        return;
    }
    int next_index = (int)as->action_index + increment;

    if (next_index >= (int)total_actions) {
        put_first_action_in_appstate(as);
    } 
    else if (next_index < 0) {
        as->action_index = (int)total_actions - 1;
        as->current_action = vector_nth(&as->all_actions, as->action_index);
    } 
    else {
        as->action_index = (size_t)next_index;
        as->current_action = vector_nth(&as->all_actions, as->action_index);
    }
    // printf("action changée\n");
}
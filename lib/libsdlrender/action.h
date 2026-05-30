#ifndef TILE_H
#define TILE_H

#include <libsdlrender/appstate.h>

void put_first_action_in_appstate(appstate_t *as);
void get_current_actions(appstate_t *as);
void pass_to_action(appstate_t *as, int increment);
void send_action_to_engine(appstate_t *as);

#endif

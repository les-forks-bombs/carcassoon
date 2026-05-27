#ifndef TILE_H
#define TILE_H

#include <sdl/appstate.h>

void put_first_action_in_appstate(AppState *as);
void get_current_actions(AppState *as);
void pass_to_action(AppState *as, int increment);
void send_action_to_engine(AppState *as);

#endif
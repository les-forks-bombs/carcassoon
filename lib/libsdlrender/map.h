#pragma once

#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/appstate.h>

void render_map(appstate_t *as);
void update_possible_places(appstate_t *as);
void print_grid(appstate_t *as);
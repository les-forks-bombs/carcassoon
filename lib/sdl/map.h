#ifndef H_MAP
#define H_MAP

#include <libcarcassonne/forward.h>
#include <sdl/appstate.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/forward.h>
#include <sdl/meeple.h>

void                render_map(appstate_t *as);
void        update_possible_places(appstate_t *as);
void print_grid(appstate_t *as);

#endif

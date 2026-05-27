#ifndef LOAD_H
#define LOAD_H

#include <sdl/appstate.h>

void load_texture(appstate_t *state, char *name, char *path);

void load_textures(appstate_t *appstate, char *directory, char *assets);

#endif

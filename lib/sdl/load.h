#ifndef LOAD_H
#define LOAD_H

#include <sdl/appstate.h>

void load_texture(AppState *state, char *name, char *path);

void load_textures(AppState *appstate, char *directory, char *assets);

#endif

#ifndef H_MAP
#define H_MAP

#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/tile_temp.h>
#include "libcarcassonne/forward.h"

void render_map(game_t *game, SDL_Renderer *renderer, camera_t *cam,SDL_Texture *test_tex);

#endif

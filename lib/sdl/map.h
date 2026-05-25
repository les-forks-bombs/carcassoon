#ifndef H_MAP
#define H_MAP

#include <libcarcassonne/forward.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/meeple.h>

void render_map(game_t *game, SDL_Renderer *renderer, camera_t *cam,
                SDL_Texture *test_tex, placed_tile_t *current_tile);

#endif

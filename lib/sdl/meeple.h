#ifndef SDL_MEEPLE_H
#define SDL_MEEPLE_H

#include <SDL3/SDL.h>
#include <libcarcassonne/forward.h>

void render_placed_meeple(placed_tile_t *tile, SDL_Renderer *renderer, float x_render, float y_render, float size_zoomed, double angle, SDL_Texture *meeple_tex);

#endif
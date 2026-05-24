#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <sdl/map.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/forward.h>
#include <sdl/resolver.h>
#include <stdio.h>
#include "sdl/consts.h"

void render_map(game_t *game, SDL_Renderer *renderer, camera_t *cam,SDL_Texture *test_tex) {
  for (int i = 0; i < MAP_TABLE_SIZE * MAP_TABLE_SIZE; i++) {
    placed_tile_t *ptt = game->map[i];
    if (ptt == NULL) continue;

    int table_x = i % MAP_TABLE_SIZE;
    int table_y = i / MAP_TABLE_SIZE;

    SDL_Texture *texture = test_tex;
    // = MAP(ptt->texture);
    SDL_Texture *meeple_tex = test_tex;

    float world_x = ((float)MAP_TABLE_SIZE-1-table_y) * MAP_TILE_SIZE;
    float world_y = ((float)MAP_TABLE_SIZE-1-table_x) * MAP_TILE_SIZE;

    float x_render    = (world_x - cam->x) * cam->zoom;
    float y_render    = (world_y - cam->y) * cam->zoom;

    float size_zoomed = MAP_TILE_SIZE * cam->zoom;

    double angle = 0.0;

    switch(ptt->orientation) {
      case LIBCARCASSONNE_TILE_ORIENTATION_WEST:
        angle = 90.0;
        break;
      case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH:
        angle = 180.0;
        break;
      case LIBCARCASSONNE_TILE_ORIENTATION_EAST:
        angle = 270.0;
        break;
      default: angle = 0.0;
    }

    if (x_render + size_zoomed > 0 && x_render < WINDOW_HEIGHT &&
        y_render + size_zoomed > 0 && y_render < WINDOW_WIDTH) {
      SDL_FRect dest = {x_render, y_render, size_zoomed, size_zoomed};
      SDL_RenderTextureRotated(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
      render_placed_meeple(ptt, renderer, x_render, y_render, size_zoomed, angle, meeple_tex);
    }
  }
}
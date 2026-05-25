#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <sdl/consts.h>
#include <sdl/map.h>
#include <sdl/meeple.h>
#include <sdl/resolver.h>
#include <stdio.h>

void render_map(game_t *game, SDL_Renderer *renderer, camera_t *cam,
                SDL_Texture *test_tex, placed_tile_t *current_tile) {
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  for (int table_y = min_coord; table_y <= max_coord; table_y++) {
    for (int table_x = min_coord; table_x <= max_coord; table_x++) {
      placed_tile_t *ptt = *game_tile_at(game, table_x, table_y);
      if (ptt == NULL) continue;

      SDL_Texture *texture = test_tex;

      float world_x = (float)(table_y)*MAP_TILE_SIZE;
      float world_y = (float)(table_x)*MAP_TILE_SIZE;

      float x_render    = (world_x - cam->x) * cam->zoom;
      float y_render    = (world_y - cam->y) * cam->zoom;
      float size_zoomed = (float)MAP_TILE_SIZE * cam->zoom;

      double angle = 0.0;
      switch (ptt->orientation) {
        case LIBCARCASSONNE_TILE_ORIENTATION_WEST:
          angle = 90.0;
          break;
        case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH:
          angle = 180.0;
          break;
        case LIBCARCASSONNE_TILE_ORIENTATION_EAST:
          angle = 270.0;
          break;
        default:
          angle = 0.0;
      }

      if (x_render + size_zoomed > 0 && x_render < WINDOW_WIDTH &&
          y_render + size_zoomed > 0 && y_render < WINDOW_HEIGHT) {
        SDL_FRect dest = {x_render, y_render, size_zoomed, size_zoomed};
        SDL_RenderTextureRotated(renderer, texture, NULL, &dest, angle, NULL,
                                 SDL_FLIP_NONE);

        if (ptt == current_tile) {
          SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
          for (int t = 0; t < 3; t++) {
            SDL_FRect border_rect = {dest.x + (float)t, dest.y + (float)t,
                                     dest.w - (float)(t * 2),
                                     dest.h - (float)(t * 2)};
            SDL_RenderRect(renderer, &border_rect);
          }
          render_possible_meeples(ptt, renderer, &dest, angle);
        }
        render_placed_meeple(ptt, renderer, &dest, angle);
      }
    }
  }
}
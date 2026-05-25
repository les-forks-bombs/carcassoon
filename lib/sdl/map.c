#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <sdl/appstate.h>
#include <sdl/consts.h>
#include <sdl/map.h>
#include <sdl/meeple.h>
#include <sdl/resolver.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "libutils/hashmap.h"

void render_map(AppState *as) {
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  for (int table_y = min_coord; table_y <= max_coord; table_y++) {
    for (int table_x = min_coord; table_x <= max_coord; table_x++) {
      placed_tile_t *ptt = *game_tile_at(&as->engine.game, table_x, table_y);
      if (ptt == NULL) continue;

      char *texturen    = ptt->parent->texture;
      char *prefix      = "/img/tiles";
      int   buffer_size = strlen(texturen) + strlen(prefix) + 2;
      char  name[buffer_size];

      snprintf(name, buffer_size, "%s/%s", prefix, texturen);

      int key_size = strlen(name) + 1;

      SDL_Texture *texture;

      SDL_Texture **texture_ptr =
          (SDL_Texture **)hashmap_get(&as->textures, name, key_size);
      if (texture_ptr != NULL) {
        texture = *texture_ptr;
      } else {
        texture = as->temp_tex;
      }

      if (texture_ptr == NULL) {
        printf("texture existe pas !!! %s \n", name);
      }

      float world_x = (float)(table_y)*MAP_TILE_SIZE;
      float world_y = (float)(table_x)*MAP_TILE_SIZE;

      float x_render    = (world_x - as->camera->x) * as->camera->zoom;
      float y_render    = (world_y - as->camera->y) * as->camera->zoom;
      float size_zoomed = (float)MAP_TILE_SIZE * as->camera->zoom;

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
        SDL_RenderTextureRotated(as->renderer, texture, NULL, &dest, angle,
                                 NULL, SDL_FLIP_NONE);

        if (ptt == as->current_tile) {
          SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);
          SDL_SetRenderDrawColor(as->renderer, 0, 255, 0, 255);
          for (int t = 0; t < 3; t++) {
            SDL_FRect border_rect = {dest.x + (float)t, dest.y + (float)t,
                                     dest.w - (float)(t * 2),
                                     dest.h - (float)(t * 2)};
            SDL_RenderRect(as->renderer, &border_rect);
          }
          render_possible_meeples(ptt, as, &dest, angle);
        }
        render_placed_meeple(ptt, as, &dest, angle);
      }
    }
  }
}
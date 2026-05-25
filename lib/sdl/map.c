#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <libcarcassonne/engine.h>
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
#include "libutils/vector.h"

void render_map(AppState *as) {
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  action_vector_t actions = engine_get_actions(&as->engine);
  vector2d_vector_t possibles_places = {0};
  vector_alloc(&possibles_places, 5);

  for (int i = 0; i < vector_size(&actions); i++) {
    action_t *action = vector_nth(&actions, i);
    if (action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
      vector2d_t pos = {.x = action->order.place_tile.x,
                        .y = action->order.place_tile.y};
      if (!vector_contains(&possibles_places, &pos)) {
        vector_append(&possibles_places, &pos);
      }
    }
  }

  for (int table_y = min_coord; table_y <= max_coord; table_y++) {
    for (int table_x = min_coord; table_x <= max_coord; table_x++) {
      float world_x = (float)(table_y)*MAP_TILE_SIZE;
      float world_y = (float)(table_x)*MAP_TILE_SIZE;

      float x_render    = (world_x - as->camera->x) * as->camera->zoom;
      float y_render    = (world_y - as->camera->y) * as->camera->zoom;
      float size_zoomed = (float)MAP_TILE_SIZE * as->camera->zoom;

      if (x_render + size_zoomed > 0 && x_render < WINDOW_WIDTH &&
          y_render + size_zoomed > 0 && y_render < WINDOW_HEIGHT) {
        SDL_FRect      dest = {x_render, y_render, size_zoomed, size_zoomed};
        placed_tile_t *ptt  = *game_tile_at(&as->engine.game, table_x, table_y);

        if (ptt != NULL) {
          // --- Rendu de la tuile existante ---
          char *texturen    = ptt->parent->texture;
          char *prefix      = "/img/tiles";
          int   buffer_size = strlen(texturen) + strlen(prefix) + 2;
          char  name[buffer_size];

          snprintf(name, buffer_size, "%s/%s", prefix, texturen);

          int           key_size = strlen(name) + 1;
          SDL_Texture  *texture;
          SDL_Texture **texture_ptr =
              (SDL_Texture **)hashmap_get(&as->textures, name, key_size);

          if (texture_ptr != NULL) {
            texture = *texture_ptr;
          } else {
            texture = as->temp_tex;
            printf("texture existe pas !!! %s \n", name);
          }

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
        } else {
          vector2d_t pos = {.x = table_x, .y = table_y};

          if (vector_contains(&possibles_places, &pos)) {
            SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(as->renderer, &dest);
          }
        }
      }
    }
  }
  vector_free(&possibles_places);
}
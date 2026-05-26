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

#include "libcarcassonne/action.h"
#include "libutils/hashmap.h"
#include "libutils/vector.h"

static double get_tile_angle(int orientation) {
  switch (orientation) {
    case LIBCARCASSONNE_TILE_ORIENTATION_WEST:  return 90.0;
    case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH: return 180.0;
    case LIBCARCASSONNE_TILE_ORIENTATION_EAST:  return 270.0;
    default:                                    return 0.0;
  }
}

static SDL_Texture *get_tile_texture(AppState *as, const tile_t *tile) {
  if (tile == NULL || tile->texture == NULL) return as->temp_tex;

  char *texturen = tile->texture;
  char *prefix = "/img/tiles";
  int buffer_size = strlen(texturen) + strlen(prefix) + 2;
  char name[buffer_size];

  snprintf(name, buffer_size, "%s/%s", prefix, texturen);
  int key_size = strlen(name) + 1;

  SDL_Texture **texture_ptr = (SDL_Texture **)hashmap_get(&as->textures, name, key_size);
  if (texture_ptr != NULL) {
    return *texture_ptr;
  }

  printf("texture existe pas !!! %s \n", name);
  return as->temp_tex;
}

static void draw_tile(AppState *as, const tile_t *tile, const SDL_FRect *dest, double angle, Uint8 alpha) {
  SDL_Texture *texture = get_tile_texture(as, tile);
  if (texture == NULL) return;

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(texture, alpha);

  SDL_RenderTextureRotated(as->renderer, texture, NULL, dest, angle, NULL, SDL_FLIP_NONE);

  SDL_SetTextureAlphaMod(texture, 255);
}

static void draw_selection_border(SDL_Renderer *renderer, const SDL_FRect *dest) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  for (int t = 0; t < 3; t++) {
    SDL_FRect border_rect = {dest->x + (float)t, dest->y + (float)t,
                             dest->w - (float)(t * 2),
                             dest->h - (float)(t * 2)};
    SDL_RenderRect(renderer, &border_rect);
  }
}

static void render_occupied_cell(AppState *as, placed_tile_t *ptt, const SDL_FRect *dest) {
  double angle = get_tile_angle(ptt->orientation);
  
  draw_tile(as, ptt->parent, dest, angle, 255);

  if (ptt == as->current_tile) {
    draw_selection_border(as->renderer, dest);
    render_possible_meeples(ptt, as, dest, angle);
  }
  render_placed_meeple(ptt, as, dest, angle);
}

static void render_empty_cell(AppState *as, int table_x, int table_y, const SDL_FRect *dest) {
  vector2d_t pos = {.x = table_x, .y = table_y};
  if (!vector_contains(&as->possibles_places, &pos)) return;

  if (as->is_waiting_for_tile && as->current_action->order.place_tile.tile != NULL && 
      as->current_action->order.place_tile.x == table_x && as->current_action->order.place_tile.y == table_y) {
    // printf("dans la boucle\n");
    double preview_angle = get_tile_angle(as->current_action->order.place_tile.orientation);
    draw_tile(as, as->current_action->order.place_tile.tile, dest, preview_angle, 160);
    /*SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, 150);
    SDL_RenderFillRect(as->renderer, dest);*/
  } else {
    SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(as->renderer, dest);
  }
}

void render_map(AppState *as) {
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  for (int table_y = min_coord; table_y <= max_coord; table_y++) {
    for (int table_x = min_coord; table_x <= max_coord; table_x++) {
      float world_x = (float)(table_y) * MAP_TILE_SIZE;
      float world_y = (float)(table_x) * MAP_TILE_SIZE;

      float x_render    = (world_x - as->camera->x) * as->camera->zoom;
      float y_render    = (world_y - as->camera->y) * as->camera->zoom;
      float size_zoomed = (float)MAP_TILE_SIZE * as->camera->zoom;

      if (x_render + size_zoomed > 0 && x_render < WINDOW_WIDTH &&
          y_render + size_zoomed > 0 && y_render < WINDOW_HEIGHT) {
        
        SDL_FRect dest = {x_render, y_render, size_zoomed, size_zoomed};
        placed_tile_t *ptt = *game_tile_at(&as->engine.game, table_x, table_y);

        if (ptt != NULL) {
          render_occupied_cell(as, ptt, &dest);
        } else {
          if(as->current_action->type==LIBCARCASSONNE_ACTION_PLACE_TILE){
            render_empty_cell(as, table_x, table_y, &dest);
          }
        }
      }
    }
  }
}

void update_possible_places(AppState *as) {
  vector_free(&as->possibles_places);
  vector_alloc(&as->possibles_places, 5);

  action_vector_t actions = engine_get_actions(&as->engine);

  for (int i = 0; i < vector_size(&actions); i++) {
    action_t *action = vector_nth(&actions, i);
    if (action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
      vector2d_t pos = {.x = action->order.place_tile.x,
                        .y = action->order.place_tile.y};
      if (!vector_contains(&as->possibles_places, &pos)) {
        vector_append(&as->possibles_places, &pos);
      }
    }
  }
}

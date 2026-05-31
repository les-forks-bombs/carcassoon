#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/forward.h>
#include <libsdlrender/map.h>
#include <libsdlrender/meeple.h>
#include <libutils/hashmap.h>
#include <libutils/vector.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "consts.h"
#include "forward.h"

static double get_tile_angle(int orientation) {
  switch (orientation) {
    case LIBCARCASSONNE_TILE_ORIENTATION_WEST:  return 90.0;
    case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH: return 180.0;
    case LIBCARCASSONNE_TILE_ORIENTATION_EAST:  return 270.0;
    default:                                    return 0.0;
  }
}

static SDL_Texture *get_tile_texture(appstate_t *as, const tile_t *tile) {
  char        *texturen    = tile->texture;
  char        *prefix      = "/img/tiles";
  unsigned int buffer_size = strlen(texturen) + strlen(prefix) + 2;
  char         name[buffer_size];

  snprintf(name, buffer_size, "%s/%s", prefix, texturen);
  unsigned int key_size = strlen(name) + 1;

  SDL_Texture **texture_ptr =
      (SDL_Texture **)hashmap_get(&as->textures, name, key_size);
  if (texture_ptr != NULL) {
    return *texture_ptr;
  }

  printf("texture existe pas !!! %s \n", name);

  return NULL;
}

static void draw_tile(appstate_t *as, const tile_t *tile, const SDL_FRect *dest,
                      double angle, Uint8 alpha) {
  SDL_Texture *texture = get_tile_texture(as, tile);
  if (texture == NULL) {
    return;
  }

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(texture, alpha);

  SDL_RenderTextureRotated(as->renderer, texture, NULL, dest, angle, NULL,
                           SDL_FLIP_NONE);

  SDL_SetTextureAlphaMod(texture, 255);
}

static void draw_selection_border(SDL_Renderer *renderer, const SDL_FRect *dest,
                                  appstate_t *as) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  if (as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE &&
      as->current_action->order.place_meeple.meeple_type != NONE) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  }
  for (int t = 0; t < 3; t++) {
    SDL_FRect border_rect = {dest->x + (float)t, dest->y + (float)t,
                             dest->w - (float)(t * 2),
                             dest->h - (float)(t * 2)};
    SDL_RenderRect(renderer, &border_rect);
  }
}

static void render_occupied_cell(appstate_t *as, placed_tile_t *ptt,
                                 const SDL_FRect *dest) {
  double angle = get_tile_angle(ptt->orientation);

  draw_tile(as, ptt->parent, dest, angle, 255);

  if (ptt == as->current_action->order.place_meeple.tile) {
    draw_selection_border(as->renderer, dest, as);
    render_possible_meeples(ptt, as, dest, angle);
  }
  render_placed_meeple(ptt, as, dest, angle);
}

static void render_empty_cell(appstate_t *as, int table_x, int table_y,
                              const SDL_FRect *dest) {
  vector2d_t pos = {.x = table_x, .y = table_y};
  if (!vector_contains(&as->possibles_places, &pos)) {
    return;
  }

  if (as->is_waiting_for_tile &&
      as->current_action->order.place_tile.tile != NULL &&
      as->current_action->order.place_tile.x == table_x &&
      as->current_action->order.place_tile.y == table_y) {
    // printf("dans la boucle\n");
    double preview_angle =
        get_tile_angle(as->current_action->order.place_tile.orientation);
    draw_tile(as, as->current_action->order.place_tile.tile, dest,
              preview_angle, 160);
    /*SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, 150);
    SDL_RenderFillRect(as->renderer, dest);*/
  } else {
    SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(as->renderer, dest);
  }
}

void draw_background(appstate_t *as) {
  SDL_Texture *texture =
      *(SDL_Texture **)hashmap_get(&as->textures, "/img/flag_of_europe.png",
                                   strlen("/img/flag_of_europe.png") + 1);

  if (texture == NULL) {
    return;
  }

  float            tex_w = 0.0F;
  float            tex_h = 0.0F;
  SDL_PropertiesID props = SDL_GetTextureProperties(texture);
  if (props != 0) {
    tex_w =
        (float)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
    tex_h =
        (float)SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
  }

  if (tex_w == 0.0F || tex_h == 0.0F) {
    return;
  }

  const float SCALE   = 0.2F;
  float       world_w = tex_w * SCALE;
  float       world_h = tex_h * SCALE;

  float world_x = -world_w / 2.0F;
  float world_y = -world_h / 2.0F;

  SDL_FRect bg = {(world_x - as->camera.x) * as->camera.zoom,
                  (world_y - as->camera.y) * as->camera.zoom,
                  world_w * as->camera.zoom, world_h * as->camera.zoom};
  SDL_RenderTexture(as->renderer, texture, NULL, &bg);
}

void render_map(appstate_t *as) {
  draw_background(as);
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  for (int table_y = min_coord; table_y <= max_coord; table_y++) {
    for (int table_x = min_coord; table_x <= max_coord; table_x++) {
      float world_x = (float)table_y * MAP_TILE_SIZE;
      float world_y = (float)table_x * MAP_TILE_SIZE;

      float x_render    = (world_x - as->camera.x) * as->camera.zoom;
      float y_render    = (world_y - as->camera.y) * as->camera.zoom;
      float size_zoomed = (float)MAP_TILE_SIZE * as->camera.zoom;

      if (x_render + size_zoomed > 0 && x_render < as->window_width &&
          y_render + size_zoomed > 0 && y_render < as->window_height) {
        SDL_FRect      dest = {x_render, y_render, size_zoomed, size_zoomed};
        placed_tile_t *ptt = *game_tile_at(&as->engine->game, table_x, table_y);

        if (ptt != NULL) {
          render_occupied_cell(as, ptt, &dest);
        } else {
          if (as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
            render_empty_cell(as, table_x, table_y, &dest);
          }
        }
      }
    }
  }
  if (as->display_grid) {
    print_grid(as);
  }
}

void update_possible_places(appstate_t *as) {
  vector_free(&as->possibles_places);
  vector_alloc(&as->possibles_places, 5);

  for (unsigned int i = 0; i < vector_size(&as->all_actions); i++) {
    action_t *action = vector_nth(&as->all_actions, i);
    if (action->type == LIBCARCASSONNE_ACTION_PLACE_TILE) {
      vector2d_t pos = {.x = action->order.place_tile.x,
                        .y = action->order.place_tile.y};
      if (!vector_contains(&as->possibles_places, &pos)) {
        vector_append(&as->possibles_places, &pos);
      }
    }
  }
}

void print_grid(appstate_t *as) {
  int min_coord = -LIBCARCASSONNE_TILES_COUNT + 1;
  int max_coord = LIBCARCASSONNE_TILES_COUNT - 1;

  float world_min_x = (float)min_coord * MAP_TILE_SIZE;
  float world_max_x = (float)(max_coord + 1) * MAP_TILE_SIZE;
  float world_min_y = (float)min_coord * MAP_TILE_SIZE;
  float world_max_y = (float)(max_coord + 1) * MAP_TILE_SIZE;

  float screen_min_x = (world_min_x - as->camera.x) * as->camera.zoom;
  float screen_max_x = (world_max_x - as->camera.x) * as->camera.zoom;
  float screen_min_y = (world_min_y - as->camera.y) * as->camera.zoom;
  float screen_max_y = (world_max_y - as->camera.y) * as->camera.zoom;

  SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 50);

  for (int table_y = min_coord; table_y <= max_coord + 1; table_y++) {
    float world_x  = (float)table_y * MAP_TILE_SIZE;
    float screen_x = (world_x - as->camera.x) * as->camera.zoom;

    if (screen_x >= 0 && screen_x <= as->window_width) {
      SDL_RenderLine(as->renderer, screen_x, screen_min_y, screen_x,
                     screen_max_y);
    }
  }

  for (int table_x = min_coord; table_x <= max_coord + 1; table_x++) {
    float world_y  = (float)table_x * MAP_TILE_SIZE;
    float screen_y = (world_y - as->camera.y) * as->camera.zoom;

    if (screen_y >= 0 && screen_y <= as->window_height) {
      SDL_RenderLine(as->renderer, screen_min_x, screen_y, screen_max_x,
                     screen_y);
    }
  }
}

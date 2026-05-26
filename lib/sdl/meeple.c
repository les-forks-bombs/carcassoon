#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>
#include <sdl/appstate.h>
#include <sdl/meeple.h>
#include "sdl/consts.h"

void render_placed_meeple(placed_tile_t *tile, AppState *as,
                          const SDL_FRect *tile_rect, double angle) {
  SDL_Texture *texture;

  SDL_Texture **texture_ptr = (SDL_Texture **)hashmap_get(
      &as->textures, "/img/meeple.svg", sizeof("/img/meeple.svg"));
  if (texture_ptr != NULL) {
    texture = *texture_ptr;
  } else {
    texture = as->temp_tex;
  }

  const tile_t *real_tile = tile->parent;
  if (real_tile == NULL || tile_rect == NULL) return;

  for (unsigned int s = 0; s < real_tile->nb_slots; s++) {
    tile_slot_t       slot       = real_tile->slots[s];
    tile_part_group_t slot_group = slot.group;

    placed_tile_group_t *ptg = tile->groups[slot_group];
    if (ptg != NULL && ptg->meeple != NULL) {
      SDL_FRect meeple_dest = calc_meeple_rect(slot, tile_rect, angle);

      player_t *player = ptg->meeple->player;
      
      if (player != NULL) {
        SDL_Color color = players_colors[player->id];
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
      }
      SDL_RenderTexture(as->renderer, texture, NULL, &meeple_dest);
    }
    SDL_SetTextureColorMod(texture, 255, 255, 255);
  }
}

// à modifier pour ne montrer que les meeples plaçables
void render_possible_meeples(placed_tile_t *tile, AppState *as,
                             const SDL_FRect *tile_rect, double angle) {
  SDL_Texture *texture;

  SDL_Texture **texture_ptr = (SDL_Texture **)hashmap_get(
      &as->textures, "/img/meeple.svg", sizeof("/img/meeple.svg"));
  if (texture_ptr != NULL) {
    texture = *texture_ptr;
  } else {
    texture = as->temp_tex;
  }
  const tile_t *real_tile = tile->parent;
  if (real_tile == NULL || tile_rect == NULL) return;

  SDL_SetTextureAlphaMod(texture, 150);
  for (unsigned int s = 0; s < real_tile->nb_slots; s++) {
    tile_slot_t       slot       = real_tile->slots[s];
    tile_part_group_t slot_group = slot.group;

    placed_tile_group_t *ptg = tile->groups[slot_group];
    if (ptg != NULL) {
      SDL_FRect meeple_dest = calc_meeple_rect(slot, tile_rect, angle);

      SDL_RenderTexture(as->renderer, texture, NULL, &meeple_dest);
    }
  }
  SDL_SetTextureAlphaMod(texture, 255);
}

static SDL_FRect calc_meeple_rect(tile_slot_t slot, const SDL_FRect *tile_rect,
                                  double angle) {
  float cx = (float)slot.x - 0.5f;
  float cy = (float)slot.y - 0.5f;

  double rad   = angle * (SDL_PI_D / 180.0);
  float  cos_a = (float)SDL_cos(rad);
  float  sin_a = (float)SDL_sin(rad);

  float rot_x = cx * cos_a - cy * sin_a;
  float rot_y = cx * sin_a + cy * cos_a;

  float final_local_x = rot_x + 0.5f;
  float final_local_y = rot_y + 0.5f;

  float meeple_size = tile_rect->w * 0.15f;

  SDL_FRect meeple_dest = {
      .x = tile_rect->x + (final_local_x * tile_rect->w) - (meeple_size / 2.0f),
      .y = tile_rect->y + (final_local_y * tile_rect->w) - (meeple_size / 2.0f),
      .w = meeple_size,
      .h = meeple_size};
  return meeple_dest;
}

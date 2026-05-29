#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <libcarcassonne/action.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/forward.h>
#include <libsdlrender/meeple.h>
#include <stdbool.h>
#include <string.h>
#include "libcarcassonne/macros.h"

static SDL_FRect calc_meeple_rect(tile_slot_t slot, const SDL_FRect *tile_rect,
                                  double angle) {
  float cx = (float)slot.x - 0.5F;
  float cy = (float)slot.y - 0.5F;

  double rad   = angle * (SDL_PI_D / 180.0);
  float  cos_a = (float)SDL_cos(rad);
  float  sin_a = (float)SDL_sin(rad);

  float rot_x = (cx * cos_a) - (cy * sin_a);
  float rot_y = (cx * sin_a) + (cy * cos_a);

  float final_local_x = rot_x + 0.5F;
  float final_local_y = rot_y + 0.5F;

  float meeple_size = tile_rect->w * 0.25F;

  SDL_FRect meeple_dest = {
      .x = tile_rect->x + (final_local_x * tile_rect->w) - (meeple_size / 2.0F),
      .y = tile_rect->y + (final_local_y * tile_rect->w) - (meeple_size / 2.0F),
      .w = meeple_size,
      .h = meeple_size};
  return meeple_dest;
}

static const char *meeple_texture = "/img/meeple.svg";

void render_placed_meeple(placed_tile_t *tile, appstate_t *as,
                          const SDL_FRect *tile_rect, double angle) {
  SDL_Texture *texture;

  SDL_Texture **texture_ptr = (SDL_Texture **)hashmap_get(
      &as->textures, meeple_texture, strlen(meeple_texture) + 1);
  if (texture_ptr != NULL) {
    texture = *texture_ptr;
  }

  const tile_t *real_tile = tile->parent;
  if (real_tile == NULL || tile_rect == NULL) {
    return;
  }

  for (unsigned int s = 0; s < real_tile->nb_slots; s++) {
    tile_slot_t       slot       = real_tile->slots[s];
    tile_part_group_t slot_group = slot.group;

    // printf("slot group : %d \n", slot_group);

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

void render_possible_meeples(placed_tile_t *tile, appstate_t *as,
                             const SDL_FRect *tile_rect, double angle) {
  SDL_Texture *texture;

  SDL_Texture **texture_ptr = (SDL_Texture **)hashmap_get(
      &as->textures, "/img/meeple.svg", sizeof("/img/meeple.svg"));

  texture = *texture_ptr;

  const tile_t *real_tile = tile->parent;
  if (real_tile == NULL || tile_rect == NULL) {
    return;
  }

  for (unsigned int s = 0; s < real_tile->nb_slots; s++) {
    tile_slot_t slot = real_tile->slots[s];
    int         g    = slot.group;

    placed_tile_group_t *ptg = tile->groups[g];

    if (ptg != NULL && as->possible_meeples[g] &&
        as->current_action->order.place_meeple.meeple_type != NONE) {
      SDL_Color c = players_colors[as->engine->game.current_player];
      SDL_SetTextureColorMod(texture, c.r, c.g, c.b);

      int alpha = 100;
      if (as->current_action &&
          as->current_action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE &&
          as->current_action->order.place_meeple.tile == tile &&
          as->current_action->order.place_meeple.part_group == g) {
        alpha = 255;
      }

      SDL_SetTextureAlphaMod(texture, alpha);

      SDL_FRect meeple_dest = calc_meeple_rect(slot, tile_rect, angle);
      SDL_RenderTexture(as->renderer, texture, NULL, &meeple_dest);
    }
  }

  SDL_SetTextureColorMod(texture, 255, 255, 255);
  SDL_SetTextureAlphaMod(texture, 255);
}

void update_possible_meeples(appstate_t *as) {

  memset(as->possible_meeples, 0, SIZE(as->possible_meeples)*sizeof(bool));

  for (unsigned int i = 0; i < vector_size(&as->all_actions); i++) {
    action_t *action = vector_nth(&as->all_actions, i);
    int       group  = action->order.place_meeple.part_group;
    as->possible_meeples[group] =
        (action->type == LIBCARCASSONNE_ACTION_PLACE_MEEPLE &&
         action->order.place_meeple.meeple_type != NONE);
  }
}

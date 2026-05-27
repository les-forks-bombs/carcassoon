#include "banner.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_image/SDL_image.h>
#include <libcarcassonne/forward.h>
#include <libutils/path.h>
#include <sdl/consts.h>
#include <sdl/forward.h>
#include <sdl/resolver.h>
#include <stdio.h>
#include <stdlib.h>

#include "text.h"

banner_t *create_banner(appstate_t *as, SDL_Color color, int nb) {
  banner_t *banner = SDL_malloc(sizeof(banner_t));
  if (!banner) return NULL;

  banner->is_open        = false;
  banner->banner_texture = NULL;
  banner->color          = color;

  banner->area.x = 20.0F + ((float)nb * 80.0F);
  banner->area.y = 0.0F;
  banner->area.w = 60.0F;
  banner->area.h = 90.0F;

  SDL_Color white = {255, 255, 255, 255};
  char *font_path = path_resolver_resolve(&resolver, "assets/fonts/Orange.ttf");

  banner->score_object =
      init_text_object(as->renderer, font_path, 24.0F, "0", white);
  banner->meeple_count_object =
      init_text_object(as->renderer, font_path, 20.0F, "5", white);
  free(font_path);

  char *img_path = path_resolver_resolve(&resolver, "assets/img/banner.svg");
  banner->banner_texture = IMG_LoadTexture(as->renderer, img_path);
  free(img_path);

  return banner;
}

banner_t **create_banner_for_each_player(appstate_t *as) {
  int nb = as->engine.game.options->players;
  if (nb <= 0) {
    return NULL;
  }

  banner_t **banners = SDL_calloc(nb, sizeof(banner_t *));
  if (banners == NULL) {
    return NULL;
  }

  banner_t *new_banner;
  for (int i = 0; i < nb; i++) {
    new_banner = create_banner(as, players_colors[i], i);
    banners[i] = new_banner;
  }
  return banners;
}

void render_banner(banner_t *banner, SDL_Renderer *renderer) {
  if (!banner) return;

  if (banner->banner_texture) {
    SDL_SetTextureColorMod(banner->banner_texture, banner->color.r,
                           banner->color.g, banner->color.b);
    SDL_RenderTexture(renderer, banner->banner_texture, NULL, &banner->area);
  }

  if (banner->score_object && banner->score_object->texture) {
    SDL_FRect text_pos = {
        banner->area.x + ((banner->area.w - banner->score_object->w) / 2.0F),
        banner->area.y + 20.0F, banner->score_object->w,
        banner->score_object->h};
    SDL_RenderTexture(renderer, banner->score_object->texture, NULL, &text_pos);
  }

  if (banner->meeple_count_object && banner->meeple_count_object->texture) {
    SDL_FRect text_pos = {
        banner->area.x +
            ((banner->area.w - banner->meeple_count_object->w) / 2.0F),
        banner->area.y + 50.0F, banner->meeple_count_object->w,
        banner->meeple_count_object->h};
    SDL_RenderTexture(renderer, banner->meeple_count_object->texture, NULL,
                      &text_pos);
  }
}

void toggle_banner(banner_t *banner, SDL_Renderer *renderer) {
  banner->area.h = (int)banner->is_open ? 90.0F : 120.0F;

  banner->is_open = ((!banner->is_open) != 0);

  if (banner->banner_texture) {
    SDL_DestroyTexture(banner->banner_texture);
  }

  char *path =
      (int)banner->is_open
          ? path_resolver_resolve(&resolver, "assets/img/banner_tall.svg")
          : path_resolver_resolve(&resolver, "assets/img/banner.svg");
  ;
  banner->banner_texture = IMG_LoadTexture(renderer, path);
  free(path);
}

void destroy_banner(banner_t *banner) {
  if (!banner) return;
  destroy_text_object(banner->score_object);
  destroy_text_object(banner->meeple_count_object);
  SDL_DestroyTexture(banner->banner_texture);
  free(banner);
}

static void update_text_value(text_object_t *text_obj, int value,
                              SDL_Renderer *renderer) {
  char buffer[6];
  SDL_snprintf(buffer, sizeof(buffer), "%d", value);

  if (text_obj->content) {
    SDL_free(text_obj->content);
  }
  text_obj->content = SDL_strdup(buffer);
  update_text_object(text_obj, renderer);
}

void synchronize_banners(appstate_t *as) {
  for (int i = 0; i < as->engine.game.options->players; i++) {
    banner_t *banner = as->banners[i];

    update_text_value(banner->score_object, as->engine.game.players[i].score,
                      as->renderer);

    for (int j = 0; j < vector_size(&as->engine.game.players[i].meeples_count);
         j++) {
      meeple_count_t meeple =
          *vector_nth(&as->engine.game.players[i].meeples_count, j);
      if (meeple.meeple_type == BASIC) {
        printf("%d meeples\n", meeple.count);
        update_text_value(banner->meeple_count_object, meeple.count,
                          as->renderer);
      }
    }
    if (as->engine.game.current_player == i && !banner->is_open) {
      toggle_banner(banner, as->renderer);
    } else if (as->engine.game.current_player != i && banner->is_open) {
      toggle_banner(banner, as->renderer);
    }
  }
}
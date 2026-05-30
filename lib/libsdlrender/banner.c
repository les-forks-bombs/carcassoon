#include "banner.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3_image/SDL_image.h>
#include <libcarcassonne/forward.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/forward.h>
#include <libutils/path.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libsdlrender/text.h"
#include "libutils/hashmap.h"
#include "libutils/vector.h"

banner_t *create_banner(appstate_t *as, banner_t *banner, SDL_Color color,
                        unsigned int nb) {
  if (!banner) {
    return NULL;
  }

  banner->is_open = false;
  banner->color   = color;

  banner->area.x = 20.0F + ((float)nb * 80.0F);
  banner->area.y = 0.0F;
  banner->area.w = 60.0F;
  banner->area.h = 90.0F;

  SDL_Color white = {255, 255, 255, 255};

  text_object_create(as, &banner->score_object, "0", white, 24.0F);
  text_object_create(as, &banner->meeple_count_object, "5", white, 20.0F);

  return banner;
}

void render_banner(appstate_t *as, banner_t *banner) {
  if (!banner) {
    return;
  }

  char *path =
      (int)banner->is_open ? "/img/banner_tall.png" : "/img/banner.png";

  SDL_Texture *texture =
      *(SDL_Texture **)hashmap_get(&as->textures, path, strlen(path) + 1);
  if (texture == NULL) {
    printf("texture not found %s !!!\n", path);
  }

  SDL_SetTextureColorMod(texture, banner->color.r, banner->color.g,
                         banner->color.b);
  SDL_RenderTexture(as->renderer, texture, NULL, &banner->area);

  SDL_FRect score_position = {
      .x = banner->area.x + ((banner->area.w - banner->score_object.w) / 2.0F),
      .y = banner->area.y + 20.0F,
      .w = banner->score_object.w,
      .h = banner->score_object.h};
  SDL_RenderTexture(as->renderer, banner->score_object.texture, NULL,
                    &score_position);

  SDL_FRect meeples_position = {
      banner->area.x +
          ((banner->area.w - banner->meeple_count_object.w) / 2.0F),
      banner->area.y + 50.0F, banner->meeple_count_object.w,
      banner->meeple_count_object.h};
  SDL_RenderTexture(as->renderer, banner->meeple_count_object.texture, NULL,
                    &meeples_position);
}

void toggle_banner(banner_t *banner) {
  banner->area.h  = (int)banner->is_open ? 90.0F : 120.0F;
  banner->is_open = ((!banner->is_open) != 0);
}

void destroy_banner(banner_t *banner) {
  if (!banner) {
    return;
  }
  text_object_free(&banner->score_object);
  text_object_free(&banner->meeple_count_object);
}

static void update_text_value(appstate_t *as, text_object_t *text,
                              unsigned int value) {
  unsigned int chars = (unsigned int)floor(log10(value));
  if (value == 0) {
    chars = 1;
  }
  chars += 1;

  char *buffer = calloc(chars + 1, sizeof(char));

  snprintf(buffer, chars + 1, "%d", value);

  if (text->content) {
    free(text->content);
  }

  text->content = buffer;

  text_object_update(as, text);
}

void synchronize_banners(appstate_t *as) {
  for (unsigned int i = 0; i < as->engine->game.options->players; i++) {
    banner_t *banner = &as->banners[i];

    update_text_value(as, &banner->score_object,
                      as->engine->game.players[i].score);

    for (unsigned int j = 0;
         j < vector_size(&as->engine->game.players[i].meeples_count); j++) {
      meeple_count_t meeple =
          *vector_nth(&as->engine->game.players[i].meeples_count, j);
      if (meeple.meeple_type == BASIC) {
        printf("%d meeples\n", meeple.count);
        update_text_value(as, &banner->meeple_count_object, meeple.count);
      }
    }

    if (as->engine->game.current_player == i) {
      toggle_banner(banner);
    }
  }
}

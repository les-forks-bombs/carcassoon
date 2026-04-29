#include "banner.h"

#include <stdlib.h>

#include "libutils/path.h"
#include "sdl/resolver.h"
#include "text.h"

banner_t *create_banner(SDL_Renderer *renderer, SDL_Color color, int nb) {
  banner_t *banner = SDL_malloc(sizeof(banner_t));
  if (!banner) return NULL;
  banner->is_open        = false;
  banner->score          = 0;
  banner->last_score     = -1;
  banner->banner_texture = NULL;
  banner->color          = color;

  banner->area.x = 20.0f + (float)nb * 80.0f;
  banner->area.y = 0.0f;
  banner->area.w = 60.0f;
  banner->area.h = 90.0f;

  SDL_Color white = {255, 255, 255, 255};

  char *font_path = path_resolver_resolve(&resolver, "assets/fonts/Orange.ttf");
  banner->score_object =
      init_text_object(renderer, font_path, 24.0f, "0", white);
  free(font_path);

  char *img_path = path_resolver_resolve(&resolver, "assets/img/banner.svg");
  banner->banner_texture = IMG_LoadTexture(renderer, img_path);
  free(img_path);

  return banner;
}

void render_banner(banner_t *banner, SDL_Renderer *renderer) {
  if (!banner) return;

  if (banner->score != banner->last_score) {
    char buffer[4];
    SDL_snprintf(buffer, sizeof(buffer), "%d", banner->score);

    if (banner->score_object->content) {
      SDL_free(banner->score_object->content);
    }

    banner->score_object->content = SDL_strdup(buffer);
    update_text_object(banner->score_object, renderer);

    banner->last_score = banner->score;
  }

  if (banner->banner_texture) {
    SDL_SetTextureColorMod(banner->banner_texture, banner->color.r,
                           banner->color.g, banner->color.b);
    SDL_RenderTexture(renderer, banner->banner_texture, NULL, &banner->area);
  }

  if (banner->score_object && banner->score_object->texture) {
    SDL_FRect text_pos = {
        banner->area.x + (banner->area.w - banner->score_object->w) / 2.0f,
        banner->area.y + 20.0f, banner->score_object->w,
        banner->score_object->h};
    SDL_RenderTexture(renderer, banner->score_object->texture, NULL, &text_pos);
  }
}

void toggle_banner(banner_t *banner, SDL_Renderer *renderer) {
  banner->area.h = banner->is_open ? 90.0f : 120.0f;

  banner->is_open = !banner->is_open;

  if (banner->banner_texture) SDL_DestroyTexture(banner->banner_texture);

  // char *path = banner->is_open ? path_resolver_resolve(&as->resolver,
  // "assets/img/banner.svg") : path_resolver_resolve(&as->resolver,
  // "assets/img/banner_tall.svg");;
  banner->banner_texture =
      IMG_LoadTexture(renderer, "lib/sdl/assets/img/carcassonne.jpg");
  // free(path);
}

void destroy_banner(banner_t *banner) {
  destroy_text_object(banner->score_object);
  free(banner->banner_texture);
  free(banner);
}

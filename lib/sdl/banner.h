#ifndef BANNER_H
#define BANNER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <sdl/appstate.h>
#include <sdl/forward.h>
#include <stdbool.h>

#include "text.h"

struct banner {
  bool           is_open;
  SDL_Color      color;
  SDL_FRect      area;
  SDL_Texture   *banner_texture;
  text_object_t *score_object;
  text_object_t *meeple_count_object;
};

banner_t  *create_banner(appstate_t *as, SDL_Color color, int nb);
banner_t **create_banner_for_each_player(appstate_t *as);
void       render_banner(banner_t *banner, SDL_Renderer *renderer);
void       toggle_banner(banner_t *banner, SDL_Renderer *renderer);
void       destroy_banner(banner_t *banner);
void       synchronize_banners(appstate_t *as);

#endif

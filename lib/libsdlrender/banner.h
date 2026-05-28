#ifndef BANNER_H
#define BANNER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <libsdlrender/text.h>
#include <stdbool.h>

#include "libsdlrender/forward.h"

struct banner {
  bool          is_open;
  SDL_Color     color;
  SDL_FRect     area;
  text_object_t score_object;
  text_object_t meeple_count_object;
};

banner_t *create_banner(appstate_t *as, banner_t *banner, SDL_Color color,
                        unsigned int nb);
void      render_banner(appstate_t *as, banner_t *banner);
void      toggle_banner(banner_t *banner);
void      destroy_banner(banner_t *banner);
void      synchronize_banners(appstate_t *as);

#endif

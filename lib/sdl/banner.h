#ifndef BANNER_H
#define BANNER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

#include "text.h"

typedef struct banner {
  text_object_t *score_object;
  int            score, last_score;
  SDL_Color      color;
  SDL_Texture   *banner_texture;
  SDL_FRect      area;
  bool           is_open;
} banner_t;

banner_t *create_banner(SDL_Renderer *renderer, SDL_Color color, int nb);
void      render_banner(banner_t *banner, SDL_Renderer *renderer);
void      toggle_banner(banner_t *banner, SDL_Renderer *renderer);
void      destroy_banner(banner_t *banner);

#endif

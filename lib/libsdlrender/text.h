#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libsdlrender/forward.h>

struct text_object {
  SDL_Texture *texture;
  float        font_size;
  SDL_Color    color;
  char        *content;
  float        w, h;
};

void text_object_create(appstate_t *appstate, text_object_t *text,
                        const char *content, SDL_Color color, float font_size);

void text_object_update(appstate_t *appstate, text_object_t *to);

void text_object_free(text_object_t *text);

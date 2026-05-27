#ifndef H_TEXT
#define H_TEXT

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "sdl/forward.h"

struct text_object {
  SDL_Texture *texture;
  float        font_size;
  SDL_Color    color;
  TTF_Font    *font;
  char        *content;
  float        w, h;
};

text_object_t *init_text_object(SDL_Renderer *renderer, char *font_path,
                                float font_size, const char *content,
                                SDL_Color color);
void           update_text_object(text_object_t *to, SDL_Renderer *renderer);
void           destroy_text_object(text_object_t *to);

#endif

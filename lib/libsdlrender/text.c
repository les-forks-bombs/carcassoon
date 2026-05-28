#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/forward.h>
#include <libsdlrender/text.h>
#include <string.h>

void text_object_create(appstate_t *appstate, text_object_t *text,
                        const char *content, SDL_Color color, float font_size) {
  memset(text, 0, sizeof(text_object_t));
  text->font_size = font_size;
  text->color     = color;
  text->content   = SDL_strdup(content);
  text->texture   = NULL;
  text_object_update(appstate, text);
}

void text_object_update(appstate_t *appstate, text_object_t *to) {
  if (!to) {
    return;
  }

  SDL_Surface *surf =
      TTF_RenderText_Blended(appstate->font, to->content, 0, to->color);
  if (!surf) {
    return;
  }

  if (to->texture) {
    SDL_DestroyTexture(to->texture);
  }

  to->texture = SDL_CreateTextureFromSurface(appstate->renderer, surf);

  if (to->texture) {
    to->w = (float)surf->w;
    to->h = (float)surf->h;
  }

  SDL_DestroySurface(surf);
}

void text_object_free(text_object_t *text) {
  if (!text) {
    return;
  }
  if (text->texture) {
    SDL_DestroyTexture(text->texture);
  }
  if (text->content) {
    free(text->content);
  }
}

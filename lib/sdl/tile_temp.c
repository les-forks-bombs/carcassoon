#define _POSIX_C_SOURCE 200809L
#include "tile_temp.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "consts.h"

tile_temp_t *create_tt(SDL_Renderer *renderer, char *chemin) {
  tile_temp_t *t = malloc(sizeof(tile_temp_t));
  if (!t) return NULL;

  t->texture = IMG_LoadTexture(renderer, chemin);
  if (!t->texture) {
    SDL_Log("Erreur chargement %s: %s", chemin, SDL_GetError());
    free(t);
    return NULL;
  }
  t->rotation = 0;
  return t;
}

void render_tt(SDL_Renderer *renderer, tile_temp_t *t) {
  if (!t || !t->texture) return;

  SDL_FRect dest = {t->x, t->y, MAP_TILE_SIZE, MAP_TILE_SIZE};

  SDL_RenderTextureRotated(renderer, t->texture, NULL, &dest, t->rotation, NULL,
                           SDL_FLIP_NONE);
}

void destroy_tt(tile_temp_t *t) {
  free(t);
}

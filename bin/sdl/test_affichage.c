#define _POSIX_C_SOURCE 200809L
#include "test_affichage.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

tuile_affichage_t *create_ta(SDL_Renderer *renderer, char *chemin) {
  tuile_affichage_t *t = malloc(sizeof(tuile_affichage_t));
  if (!t) return NULL;

  t->texture = IMG_LoadTexture(renderer, chemin);
  if (!t->texture) {
    SDL_Log("Erreur chargement %s: %s", chemin, SDL_GetError());
    free(t);
    return NULL;
  }

  t->chemin = strdup(chemin);
  SDL_GetTextureSize(t->texture, &t->w, &t->h);
  t->rotation = 0;
  return t;
}

void rotate_ta(tuile_affichage_t *t, int rot) { t->rotation = rot; }

void move_ta(tuile_affichage_t *t, float x, float y) {
  t->x += x;
  t->y += y;
}

void redimensionate_ta(tuile_affichage_t *t, double w, double h) {
  t->h = h;
  t->w = w;
}

void render_ta(SDL_Renderer *renderer, tuile_affichage_t *t) {
  if (!t || !t->texture) return;

  SDL_FRect dest = {t->x, t->y, t->w, t->h};

  SDL_RenderTextureRotated(renderer, t->texture, NULL, &dest, t->rotation, NULL,
                           SDL_FLIP_NONE);
}

void destroy_ta(tuile_affichage_t *t) {
  free(t->chemin);
  free(t);
}

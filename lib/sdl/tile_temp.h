#ifndef TILE_TEMP
#define TILE_TEMP
#include <SDL3_image/SDL_image.h>

typedef struct tile_temp {
  SDL_Texture *texture;
  float        x, y;
  float        world_x, world_y;
  double       rotation;
} tile_temp_t;

tile_temp_t *create_tt(SDL_Renderer *renderer, char *chemin);
void               render_tt(SDL_Renderer *renderer, tile_temp_t *t);
void               rotate_tt(tile_temp_t *t, int rot);
void               redimensionate_tt(tile_temp_t *t, double w, double h);
void               move_tt(tile_temp_t *t, float x, float y);
void               destroy_tt(tile_temp_t *at);

#endif

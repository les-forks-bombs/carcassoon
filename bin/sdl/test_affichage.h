#ifndef TEST
#define TEST
#include <SDL3_image/SDL_image.h>

typedef struct tuile_affichage {
    SDL_Texture *texture;
    char *chemin;
    float x,y,w,h;
    double rotation;
} tuile_affichage_t;

tuile_affichage_t *create_ta(SDL_Renderer *renderer,char *chemin);
void render_ta(SDL_Renderer *renderer, tuile_affichage_t *t);
void rotate_ta(tuile_affichage_t *t,int rot);
void destroy_ta(tuile_affichage_t *t);

#endif

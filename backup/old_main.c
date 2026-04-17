#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_affichage.h"

int main(int argc, char *argv[]) {
  SDL_Window   *window    = NULL;
  SDL_Renderer *renderer  = NULL;
  SDL_Texture  *maTexture = NULL;
  int           statut    = EXIT_FAILURE;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Ma Fenetre SDL3", 1000, 800, 0, &window,
                                   &renderer)) {
    SDL_Log("Erreur Window/Renderer : %s", SDL_GetError());
    goto Quit;
  }

  tuile_affichage_t *test =
      create_ta(renderer, "assets/img/tiles_png/tile_00.png");
  rotate_ta(test, 90);

  if (maTexture == NULL) {
    SDL_Log("Erreur chargement : %s", SDL_GetError());
  } else {
    SDL_RenderClear(renderer);
    render_ta(renderer, test);
    SDL_RenderPresent(renderer);
  }

  SDL_Event event;
  int       running = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = 0;
      }
    }
    SDL_RenderClear(renderer);
    render_ta(renderer, test);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  statut = EXIT_SUCCESS;

  SDL_DestroyTexture(maTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  destroy_ta(test);

Quit:
  SDL_Quit();
  return statut;
}

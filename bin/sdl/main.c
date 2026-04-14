#include <SDL3/SDL.h>
// #include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *maTexture = NULL;
    int statut = EXIT_FAILURE;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // IMG_Init(IMG_INIT_JPG);

    if (!SDL_CreateWindowAndRenderer("Ma Fenetre SDL3", 640, 480, 0, &window, &renderer))
    {
        SDL_Log("Erreur Window/Renderer : %s", SDL_GetError());
        goto Quit;
    }

    /* maTexture = IMG_LoadTexture(renderer, "img/carcassonne.jpg");

    if (maTexture == NULL) {
        SDL_Log("Erreur chargement : %s", SDL_GetError());
    }
    else {
      SDL_RenderClear(renderer);
      SDL_RenderTexture(renderer, maTexture, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
    */

    SDL_Event event;
    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, maTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); 
    }


    statut = EXIT_SUCCESS;

    // SDL_DestroyTexture(maTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

Quit:
    // IMG_Quit();
    SDL_Quit();
    return statut;
}

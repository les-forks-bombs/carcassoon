#include <libcarcassonne/libcarcassonne.h>
#include <SDL3/SDL.h>

#define WINDOW_H 500
#define WINDOW_W 500

int main(void) {
    SDL_Window *w = NULL;
    SDL_Renderer *r = NULL;

    // initialization of the sdl library --- this is done at startup
    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (ret < 0) {
        SDL_Log("SDL_Init() Error: %s", SDL_GetError());
        return -1;
    }

    // create the window using sdl
    w = SDL_CreateWindow("Carcassonne omg omg", WINDOW_H, WINDOW_W, 0);
    if (!w) {
        SDL_Log("SDL_CreateWindow() Error: %s", SDL_GetError());
        return -1;
    }

    // creation du renderer (on utilisera pas opengl ou vulkan)
    r = SDL_CreateRenderer(w, NULL);
    if (!r) {
        SDL_Log("SDL_CreateRenderer() Error: %s", SDL_GetError());
        return -1;
    }

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                case SDL_EVENT_QUIT: // triggered on window close
                    running = 0;
                    break;
                case SDL_EVENT_KEY_DOWN: // triggered when user presses ESC key
                    if (event.key.key == SDLK_ESCAPE) {
                        running = 0;
                    }
                    break;
            }
        }

        SDL_RenderClear(r);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDebugText(r, 50, 50, "omg carcassonne");
        SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
        SDL_RenderPresent(r);
    }
    

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
    SDL_Quit();
}

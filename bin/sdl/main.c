#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_FRect mouseposrect;
static Uint64 last_time = 0;
static int frame_count = 0;
static int current_fps = 0;
static SDL_Color color;

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint8 r;
    frame_count++;
    Uint64 current_time = SDL_GetTicks();
    if (current_time > last_time + 1000) {
        current_fps = frame_count; // On sauvegarde le total
        frame_count = 0;           // On remet le compteur à zéro
        last_time = current_time;  // On met à jour le chronomètre
    }

    /* fade between shades of red every 3 seconds, from 0 to 255. */
    r = (Uint8) ((((float) (SDL_GetTicks() % 3000)) / 3000.0f) * 255.0f);
    SDL_SetRenderDrawColor(renderer, color.a, color.b, color.g, color.r);

    /* you have to draw the whole window every frame. Clearing it makes sure the whole thing is sane. */
    SDL_RenderClear(renderer);  /* clear whole window to that fade color. */

    /* set the color to white */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /* draw a square where the mouse cursor currently is. */
    SDL_RenderFillRect(renderer, &mouseposrect);

    SDL_RenderDebugText(renderer, 25, 25, "OMG! C'est carcassonne !");
    SDL_RenderDebugTextFormat(renderer, 25, 50, "FPS: %d", current_fps);

    /* put everything we drew to the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:  /* triggers on last window close and other things. End the program. */
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:  /* quit if user hits ESC key */
            if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
                return SDL_APP_SUCCESS;
            }
            if (event->key.scancode == SDL_SCANCODE_A) {
                color.a = (color.a + 10) % 255;
            }
            if (event->key.scancode == SDL_SCANCODE_G) {
                color.g = (color.g + 10) % 255;
            }
            if (event->key.scancode == SDL_SCANCODE_B) {
                color.b = (color.b + 10) % 255;
            }
            break;

        case SDL_EVENT_MOUSE_MOTION:  /* keep track of the latest mouse position */
            /* center the square where the mouse is */
            mouseposrect.x = event->motion.x - (mouseposrect.w / 2);
            mouseposrect.y = event->motion.y - (mouseposrect.h / 2);
            break;
        
        case SDL_EVENT_MOUSE_BUTTON_UP:
            color.b = (color.b + 50) % 255;
            color.g = (color.g + 50) % 255;
            color.r = (color.r + 50) % 255;
            break;
            
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Carcassonne", "1.0", "eu.telecomnancy.ppii.fork-bombs.carcassonne");
    

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Carcassonne", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);

    mouseposrect.x = mouseposrect.y = -1000;  /* -1000 so it's offscreen at start */
    mouseposrect.w = mouseposrect.h = 50;

    color.a = 1;
    color.b = 0;
    color.g = 0;
    color.r = 0;

    last_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
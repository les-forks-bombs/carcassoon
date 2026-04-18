#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

#include "test_affichage.h"

#define WINDOW_WIDTH              800
#define WINDOW_HEIGHT             500
#define WINDOW_RESIZABLE          0
#define STEP_RATE_IN_MILLISECONDS 16

typedef struct {
  SDL_Window        *window;
  SDL_Renderer      *renderer;
  Uint64             last_step;
  tuile_affichage_t *tuile;
} AppState;

static SDL_AppResult handle_key_event_(void *appstate, SDL_Keycode key_val) {
  AppState *as = (AppState *)appstate;
  switch (key_val) {
    case SDLK_ESCAPE:
      return SDL_APP_SUCCESS;
    case SDLK_SPACE:
      redimensionate_ta(as->tuile, 80, 80);
      break;
    case SDLK_BACKSPACE:
      redimensionate_ta(as->tuile, 50, 50);
      break;
    case SDLK_UP:
      move_ta(as->tuile, 0, -10.0f);
      break;
    case SDLK_DOWN:
      move_ta(as->tuile, 0, 10.0f);
      break;
    case SDLK_LEFT:
      move_ta(as->tuile, -10.0f, 0);
      break;
    case SDLK_RIGHT:
      move_ta(as->tuile, 10.0f, 0);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState    *as  = (AppState *)appstate;
  const Uint64 now = SDL_GetTicks();

  while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
    as->last_step += STEP_RATE_IN_MILLISECONDS;
  }
  SDL_RenderClear(as->renderer);
  render_ta(as->renderer, as->tuile);
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)appstate;
  (void)argc;
  (void)argv;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }

  AppState *as = (AppState *)SDL_malloc(sizeof(AppState));
  if (!as) {
    return SDL_APP_FAILURE;
  }

  *appstate = as;

  if (!SDL_CreateWindowAndRenderer("Test fenêtre Carcassonne", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, WINDOW_RESIZABLE, &as->window,
                                   &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  as->tuile = create_ta(as->renderer, "assets/img/tiles_png/tile_00.png");

  if (!as->tuile) {
    return SDL_APP_FAILURE;
  }

  redimensionate_ta(as->tuile, 50, 50);

  as->last_step = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  (void)appstate;
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
      return handle_key_event_(appstate, event->key.key);
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)appstate;
  (void)result;

  if (appstate != NULL) {
    AppState *as = (AppState *)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    SDL_free(as);
  }
}

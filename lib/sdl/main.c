#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

#include "consts.h"
#include "tile_temp.h"
#include "camera.h"
#include "map.h"

typedef struct {
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  map_t         *map;
  camera_t      *camera;
  SDL_FRect      map_viewport;
  Uint64         last_step;
} AppState;

static SDL_AppResult handle_key_event_(void *appstate, SDL_Keycode key_val) {
  AppState *as = (AppState *)appstate;
  switch (key_val) {
    case SDLK_ESCAPE:
      return SDL_APP_SUCCESS;
    case SDLK_UP:
      as->camera->y += 10.0f;
      break;
    case SDLK_DOWN:
      as->camera->y -= 10.0f;
      break;
    case SDLK_LEFT:
      as->camera->x += 10.0f;
      break;
    case SDLK_RIGHT:
      as->camera->x -= 10.0f;
      break;
    case SDLK_KP_PLUS:
      as->camera->zoom += 0.1f;
      break;
    case SDLK_KP_MINUS:
      as->camera->zoom -= 0.1f;
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
  
  SDL_Rect v = { (int)as->map_viewport.x, (int)as->map_viewport.y, 
                 (int)as->map_viewport.w, (int)as->map_viewport.h };
  SDL_SetRenderViewport(as->renderer, &v);

  render_map(as->map, as->renderer, as->camera);

  SDL_SetRenderViewport(as->renderer, NULL);
  
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)appstate;
  (void)argc;
  (void)argv;

  if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

  AppState *as = (AppState *)SDL_malloc(sizeof(AppState));
  if (!as) return SDL_APP_FAILURE;
  *appstate = as;

  if (!SDL_CreateWindowAndRenderer("Carcassonne Test", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  as->map = create_map();
  as->camera = create_camera();

  as->map->tiles[0] = create_tt(as->renderer, "lib/sdl/assets/img/tiles_png/tile_00.png");
  if (as->map->tiles[0]) {
      as->map->tiles[0]->world_x = 100.0f;
      as->map->tiles[0]->world_y = 100.0f;
  }
  as->map->tiles[1] = create_tt(as->renderer, "lib/sdl/assets/img/tiles_png/tile_01.png");
  if (as->map->tiles[1]) {
      as->map->tiles[1]->world_x = 164.0f;
      as->map->tiles[1]->world_y = 100.0f;
  }

  as->map_viewport.x = 0;
  as->map_viewport.y = 0;
  as->map_viewport.w = 500;
  as->map_viewport.h = 500;

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

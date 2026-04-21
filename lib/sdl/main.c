#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>

#include "camera.h"
#include "consts.h"
#include "map.h"
#include "text.h"
#include "tile_temp.h"

typedef struct {
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  map_t         *map;
  camera_t      *camera;
  SDL_FRect      map_viewport;
  Uint64         last_step;
  text_object_t *text;
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

static SDL_AppResult handle_mouse_event_(void *appstate, SDL_Event *event) {
  AppState *as = (AppState *)appstate;
  float     mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  const SDL_FPoint mouse_pos = {mouseX, mouseY};

  switch (event->type) {
    case SDL_EVENT_MOUSE_WHEEL:
      if (SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        float localX = mouseX - as->map_viewport.x;
        float localY = mouseY - as->map_viewport.y;

        float worldMouseX = (localX / as->camera->zoom) + as->camera->x;
        float worldMouseY = (localY / as->camera->zoom) + as->camera->y;

        if (event->wheel.y > 0) {
          if (as->camera->zoom < ZOOM_MAX) as->camera->zoom *= 1.1f;
        } else {
          if (as->camera->zoom > ZOOM_MIN) as->camera->zoom *= 0.9f;
        }

        as->camera->x = worldMouseX - (localX / as->camera->zoom);
        as->camera->y = worldMouseY - (localY / as->camera->zoom);
      }
      break;
    case SDL_EVENT_MOUSE_MOTION:
      if (event->motion.state & SDL_BUTTON_LMASK &&
          SDL_PointInRectFloat(&mouse_pos, &as->map_viewport)) {
        as->camera->x -= event->motion.xrel / as->camera->zoom;
        as->camera->y -= event->motion.yrel / as->camera->zoom;
      }
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

  SDL_Rect v = {(int)as->map_viewport.x, (int)as->map_viewport.y,
                (int)as->map_viewport.w, (int)as->map_viewport.h};
  SDL_SetRenderViewport(as->renderer, &v);

  render_map(as->map, as->renderer, as->camera);

  SDL_SetRenderViewport(as->renderer, NULL);

  if (as->text) {
    SDL_FRect dest = {50.0f, 50.0f, as->text->w, as->text->h};
    SDL_RenderTexture(as->renderer, as->text->texture, NULL, &dest);
  }

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

  if (!SDL_CreateWindowAndRenderer("Carcassonne Test", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, 0, &as->window,
                                   &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  if (!TTF_Init()) {
    return SDL_APP_FAILURE;
  }

  SDL_Color white_text = {255, 255, 255, 255};
  as->text = init_text_object(as->renderer, "lib/sdl/assets/fonts/Orange.ttf",
                              32.0f, "Ici c'est Carcassonne !", white_text);

  as->map    = create_map();
  as->camera = create_camera();

  if (as->map == NULL || as->camera == NULL) {
    return SDL_APP_FAILURE;
  }

  int map_width_temp = 5;

  for (int i = 0; i < 10; i++) {
    int col = i % map_width_temp;
    int row = i / map_width_temp;

    as->map->tiles[i] =
        create_tt(as->renderer, "lib/sdl/assets/img/tiles_png/tile_05.png");

    if (as->map->tiles[i]) {
      as->map->tiles[i]->world_x = (col * MAP_TILE_SIZE);
      as->map->tiles[i]->world_y = (row * MAP_TILE_SIZE);
    }
  }

  as->map_viewport.x = 0;
  as->map_viewport.y = 0;
  as->map_viewport.w = 500;
  as->map_viewport.h = 500;

  as->last_step = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *as = (AppState *)appstate;
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
      return handle_key_event_(appstate, event->key.key);
    case SDL_EVENT_MOUSE_WHEEL:
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      return handle_mouse_event_(appstate, event);
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
    destroy_text_object(as->text);
    TTF_Quit();
    SDL_free(as);
  }
}

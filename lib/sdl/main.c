#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>


#include "libcarcassonne/forward.h"
#include "libcarcassonne/options.h"
#include "libutils/path.h"
#include "sdl/resolver.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sdl/banner.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/map.h>
#include <sdl/text.h>
#include <sdl/tile_temp.h>
#include <stdlib.h>
#include "libcarcassonne/engine.h"

typedef struct {
  SDL_Window     *window;
  SDL_Renderer   *renderer;
  camera_t       *camera;
  SDL_FRect       map_viewport;
  Uint64          last_step;
  text_object_t  *text;
  engine_t engine;
  SDL_Texture *temp_tex;

  banner_t *test_banner, *test_banner2;
} AppState;

path_resolver_t resolver;

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
    case SDLK_A:
      as->test_banner->score += 1;
      break;
    case SDLK_Z:
      toggle_banner(as->test_banner2, as->renderer);
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

  SDL_SetRenderDrawColor(as->renderer, 164, 116, 73, 255);
  SDL_RenderClear(as->renderer);

  SDL_Rect v = {(int)as->map_viewport.x, (int)as->map_viewport.y,
                (int)as->map_viewport.w, (int)as->map_viewport.h};
  SDL_SetRenderViewport(as->renderer, &v);

  render_map(&as->engine.game, as->renderer, as->camera,as->temp_tex);

  SDL_SetRenderViewport(as->renderer, NULL);

  if (as->text) {
    SDL_FRect dest = {50.0f, 50.0f, as->text->w, as->text->h};
    SDL_RenderTexture(as->renderer, as->text->texture, NULL, &dest);
  }

  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderRect(as->renderer, &as->map_viewport);

  render_banner(as->test_banner, as->renderer);
  render_banner(as->test_banner2, as->renderer);

  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}


static void init_game(AppState *as){
  const tile_t* tile;

  /** Tour 1 Bastien */

  tile = deck_find_tile(&as->engine.game.deck, "FCFC", true);

  int x = 0;
  int y = 1;

  action_t action = {
      .type  = LIBCARCASSONNE_ACTION_PLACE_TILE,
      .order = {
          .place_tile = {.tile        = tile,
                         .x           = x,
                         .y           = y,
                         .orientation = LIBCARCASSONNE_TILE_ORIENTATION_WEST}}};

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = tile->parts_groups[1];
  action.order.place_meeple.tile        = *game_tile_at(&as->engine.game, x, y);
  action.order.place_meeple.x           = x;
  action.order.place_meeple.y           = y;

  /** Tour 1 Damien */

  tile = deck_find_tile(&as->engine.game.deck, "FRRR", false);

  x = 1;
  y = 0;

  action.type = LIBCARCASSONNE_ACTION_PLACE_TILE;

  action.order.place_tile.tile        = tile;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH;
  action.order.place_tile.x           = x;
  action.order.place_tile.y           = y;

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = tile->parts_groups[1];
  action.order.place_meeple.tile        = *game_tile_at(&as->engine.game, x, y);
  action.order.place_meeple.x           = x;
  action.order.place_meeple.y           = y;

  /** Tour 1 Matthieu */

  tile = deck_find_tile(&as->engine.game.deck, "FFRR", false);

  x = 1;
  y = -1;

  action.type = LIBCARCASSONNE_ACTION_PLACE_TILE;

  action.order.place_tile.tile        = tile;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_EAST;
  action.order.place_tile.x           = x;
  action.order.place_tile.y           = y;

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = tile->parts_groups[3];
  action.order.place_meeple.tile        = *game_tile_at(&as->engine.game, x, y);
  action.order.place_meeple.x           = x;
  action.order.place_meeple.y           = y;

  /** Tour 2 Bastien */

  tile = deck_find_tile(&as->engine.game.deck, "FRFR", false);

  x = 2;
  y = 0;

  action.type = LIBCARCASSONNE_ACTION_PLACE_TILE;

  action.order.place_tile.tile        = tile;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_EAST;
  action.order.place_tile.x           = x;
  action.order.place_tile.y           = y;

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = tile->parts_groups[3];
  action.order.place_meeple.tile        = *game_tile_at(&as->engine.game, -1, 0);
  action.order.place_meeple.x           = x;
  action.order.place_meeple.y           = y;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

  create_path_resolver(&resolver);
  options_t options = parse_options(argc,argv);

  if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

  AppState *as = (AppState *)SDL_calloc(1,sizeof(AppState));
  if (!as) return SDL_APP_FAILURE;
  *appstate = as;

  return_code_t init = create_engine(&as->engine, options);

  if (init!=0){
    return SDL_APP_FAILURE;
  }

  init_game(as);

  // pour resolve:
  char *path = path_resolver_resolve(&resolver, "assets/img/carcassonne.jpg");
  printf("path relatif: %s\n", path);
  free(path);

  if (!SDL_CreateWindowAndRenderer("Carcassonne Test", WINDOW_WIDTH,
                                   WINDOW_HEIGHT, 0, &as->window,
                                   &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderVSync(as->renderer, true);

  if (!TTF_Init()) {
    return SDL_APP_FAILURE;
  }

  SDL_Color white_text = {255, 255, 255, 255};
  path = path_resolver_resolve(&resolver, "assets/fonts/Orange.ttf");
  printf("path relatif: %s\n", path);

  as->text = init_text_object(as->renderer, path, 32.0f,
                              "Ici c'est Carcassonne !", white_text);
  free(path);

  as->camera = create_camera();

  if (as->camera == NULL) {
    return SDL_APP_FAILURE;
  }

  as->map_viewport.x = 100;
  as->map_viewport.y = 150;
  as->map_viewport.w = 800;
  as->map_viewport.h = 400;

  path = path_resolver_resolve(&resolver, "assets/img/tiles/tile_01.svg");
  SDL_Texture *tex = IMG_LoadTexture(as->renderer, path);
  as->temp_tex = tex;

  SDL_Color blue         = {0, 0, 255, 255};
  banner_t *test_banner  = create_banner(as->renderer, blue, 1);
  banner_t *test_banner2 = create_banner(as->renderer, blue, 2);

  as->test_banner  = test_banner;
  as->test_banner2 = test_banner2;

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
    free_options(&as->engine.config);
    destroy_engine(&as->engine);
    SDL_DestroyTexture(as->temp_tex);
    SDL_free(as);
  }
}

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>

#include "libcarcassonne/forward.h"
#include "libcarcassonne/options.h"
#include "libutils/hashmap.h"
#include "libutils/path.h"
#include "sdl/resolver.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game.h>
#include <sdl/banner.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/map.h>
#include <sdl/meeple.h>
#include <sdl/text.h>
#include <stdlib.h>

typedef HashMap(char *, ) textures_hashmap_t;

typedef struct {
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  camera_t      *camera;
  SDL_FRect      map_viewport;
  Uint64         last_step;
  text_object_t *text;
  engine_t       engine;
  SDL_Texture   *temp_tex;
  placed_tile_t *current_tile;

  banner_t **banners;

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
      as->banners[0]->score += 1;
      break;
    case SDLK_Z:
      toggle_banner(as->banners[1], as->renderer);
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

  render_map(&as->engine.game, as->renderer, as->camera, as->temp_tex,
             as->current_tile);

  SDL_SetRenderViewport(as->renderer, NULL);

  if (as->text) {
    SDL_FRect dest = {50.0f, 50.0f, as->text->w, as->text->h};
    SDL_RenderTexture(as->renderer, as->text->texture, NULL, &dest);
  }

  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderRect(as->renderer, &as->map_viewport);

  for (int nb_players = 0; nb_players < as->engine.game.options->players;
       nb_players++) {
    render_banner(as->banners[nb_players], as->renderer);
  }

  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

static void init_game(AppState *as) {
  const tile_t *tile;
  action_t      action = {0};

  // 10 tours avec tuiles, positions, orientations et part_groups valides
  // Basé sur l'ancien long_play_test qui fonctionnait
  struct {
    const char   *tile_id;
    bool          blason;
    int           x, y, orientation;
    int           part_group;
    meeple_type_t meeple_type;
  } turns[] = {
      // Tour 1 - 3 joueurs
      {"FCFC", true, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B, BASIC},
      {"FRRR", false, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"FFRR", false, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH, A, BASIC},
      // Tour 2
      {"FRFR", false, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, A, BASIC},
      {"CRFR", false, -1, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, D, NONE},
      {"CCRR", false, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_WEST, B, BASIC},
      // Tour 3
      {"RRRR", false, -2, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"CCRR", false, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH, C, BASIC},
      {"CFRR", false, 1, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST, C, NONE},
      // Tour 4
      {"CRRR", false, -2, 2, LIBCARCASSONNE_TILE_ORIENTATION_WEST, A, BASIC}};

  for (int i = 0; i < 10; i++) {
    tile = deck_find_tile(&as->engine.game.deck, turns[i].tile_id,
                          turns[i].blason);

    // Placement de la tuile
    action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
    action.order.place_tile.tile        = tile;
    action.order.place_tile.x           = turns[i].x;
    action.order.place_tile.y           = turns[i].y;
    action.order.place_tile.orientation = turns[i].orientation;

    dispatch_action(&as->engine, action);

    // Placement du meeple
    action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
    placed_tile_t **placed_tile =
        game_tile_at(&as->engine.game, turns[i].x, turns[i].y);

    action.order.place_meeple.meeple_type = BASIC;
    action.order.place_meeple.part_group  = turns[i].part_group;
    action.order.place_meeple.tile        = *placed_tile;
    action.order.place_meeple.x           = turns[i].x;
    action.order.place_meeple.y           = turns[i].y;

    dispatch_action(&as->engine, action);

    as->current_tile = *placed_tile;
  }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  create_path_resolver(&resolver);
  options_t options = parse_options(argc, argv);

  if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

  AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
  if (!as) return SDL_APP_FAILURE;
  *appstate = as;

  return_code_t init = create_engine(&as->engine, options);

  if (init != 0) {
    return SDL_APP_FAILURE;
  }

  start_game(&as->engine);

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

  center_camera_on_start(as->camera, &as->map_viewport);

  path = path_resolver_resolve(&resolver, "assets/img/carcassonne.jpg");
  SDL_Texture *tex = IMG_LoadTexture(as->renderer, path);
  as->temp_tex     = tex;

  as->banners = create_banner_for_each_player(as->renderer,
                                              as->engine.game.options->players);

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
    for (int nb_players = 0; nb_players < as->engine.game.options->players;
         nb_players++) {
      destroy_banner(as->banners[nb_players]);
    }
    SDL_free(as->banners);
    SDL_free(as);
  }
}

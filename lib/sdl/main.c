#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "libcarcassonne/forward.h"
#include <sdl/forward.h>
#include "libcarcassonne/options.h"
#include "libutils/hashmap.h"
#include "libutils/path.h"
#include "sdl/resolver.h"
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <dirent.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game.h>
#include <sdl/appstate.h>
#include <sdl/banner.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/map.h>
#include <sdl/meeple.h>
#include <sdl/text.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sdl/events.h>
#include <sdl/game_test.h>
#include <sdl/load.h>
#include <sdl/action.h>

path_resolver_t resolver;

SDL_AppResult SDL_AppIterate(void *appstate) {
  appstate_t    *as  = (appstate_t *)appstate;
  const Uint64 now = SDL_GetTicks();

  as->window_width = DEFAULT_WINDOW_WIDTH;
  as->window_height = DEFAULT_WINDOW_HEIGHT;

  while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
    as->last_step += STEP_RATE_IN_MILLISECONDS;
  }

  SDL_SetRenderDrawColor(as->renderer, 164, 116, 73, 255);
  SDL_RenderClear(as->renderer);

  SDL_Rect v = {(int)as->map_viewport.x, (int)as->map_viewport.y,
                (int)as->map_viewport.w, (int)as->map_viewport.h};
  SDL_SetRenderViewport(as->renderer, &v);

  render_map(as);

  SDL_SetRenderViewport(as->renderer, NULL);

  if (as->text) {
    SDL_FRect dest = {50.0f, 50.0f, as->text->w, as->text->h};
    SDL_RenderTexture(as->renderer, as->text->texture, NULL, &dest);
  }

  /*SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderRect(as->renderer, &as->map_viewport);*/

  for (int nb_players = 0; nb_players < as->engine.game.options->players;
       nb_players++) {
    as->banners[nb_players]->score = as->engine.game.players[nb_players].score;
    if(as->engine.game.current_player == nb_players && !as->banners[nb_players]->is_open){
      toggle_banner(as->banners[nb_players],as->renderer);
    }
    else if(as->engine.game.current_player != nb_players && as->banners[nb_players]->is_open){
      toggle_banner(as->banners[nb_players],as->renderer);
    }
    render_banner(as->banners[nb_players], as->renderer);
  }
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  create_path_resolver(&resolver);
  options_t options = parse_options(argc, argv);

  if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;

  appstate_t *as = (appstate_t *)SDL_calloc(1, sizeof(appstate_t));
  if (!as) return SDL_APP_FAILURE;
  *appstate = as;

  return_code_t init = create_engine(&as->engine, options);

  if (init != 0) {
    return SDL_APP_FAILURE;
  }

  start_game(&as->engine);

  // init_game(as);

  // pour resolve:
  char *path;

  if (!SDL_CreateWindowAndRenderer("Carcassonne Test", DEFAULT_WINDOW_WIDTH,
                                   DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &as->window,
                                   &as->renderer)) {
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderVSync(as->renderer, true);

  if (!TTF_Init()) {
    return SDL_APP_FAILURE;
  }

  /*SDL_Color white_text = {255, 255, 255, 255};
  path = path_resolver_resolve(&resolver, "assets/fonts/Orange.ttf");
  // printf("path relatif: %s\n", path);

  as->text = init_text_object(as->renderer, path, 32.0f,
                              "Ici c'est Carcassonne !", white_text);
  free(path);*/

  as->camera = create_camera();

  if (as->camera == NULL) {
    return SDL_APP_FAILURE;
  }

  as->map_viewport.x = 0;
  as->map_viewport.y = 0;
  as->map_viewport.w = DEFAULT_WINDOW_WIDTH;
  as->map_viewport.h = DEFAULT_WINDOW_HEIGHT;

  hashmap_create(&as->textures, 256);

  path = path_resolver_resolve(&resolver, "assets/img/tiles/tile_00.png");
  SDL_Texture *temp_tex = IMG_LoadTexture(as->renderer, path);
  hashmap_set(&as->textures, "test", sizeof("test"), &temp_tex,
              sizeof(SDL_Texture *));
  as->temp_tex = temp_tex;

  char *assets = path_resolver_resolve(&resolver, "assets");
  char *img    = path_resolver_resolve(&resolver, "assets/img");
  load_textures(as, img, assets);
  free(assets);

  center_camera_on_start(as);

  as->banners = create_banner_for_each_player(as->renderer,
                                              as->engine.game.options->players);

  get_current_actions(as);
  put_first_action_in_appstate(as);
  update_possible_places(as);
  as->last_step = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  appstate_t *as = (appstate_t *)appstate;
  switch (event->type) {
    case SDL_EVENT_WINDOW_RESIZED:
      as->window_width = event->window.data1;
      as->window_height = event->window.data2;
      break;
    /*case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
      int new_width, new_height;
      SDL_GetCurrentRenderOutputSize(as->renderer, &new_width, &new_height);
      as->window_width = (float)new_width;
      as->window_height = (float)new_height;
      printf("New window size: %d x %d\n", new_width, new_height);
      as->map_viewport.w = as->window_width;
      as->map_viewport.h = as->window_height;
      break;*/
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
    appstate_t *as = (appstate_t *)appstate;
    SDL_DestroyRenderer(as->renderer);
    SDL_DestroyWindow(as->window);
    destroy_text_object(as->text);
    free_options(&as->engine.config);
    destroy_engine(&as->engine);
    SDL_DestroyTexture(as->temp_tex);
    for (int nb_players = 0; nb_players < as->engine.game.options->players;
         nb_players++) {
      destroy_banner(as->banners[nb_players]);
    }
    SDL_free(as->banners);
    vector_free(&as->possibles_places);
    hashmap_free(&as->textures);
    SDL_free(as);
    TTF_Quit();

  }
}

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libutils/hashmap.h>
#include <libutils/path.h>
#include <libutils/vector.h>
#include <sdl/forward.h>
#include <sdl/resolver.h>
#include <stdbool.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sdl/action.h>
#include <sdl/banner.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/events.h>
#include <sdl/load.h>
#include <sdl/map.h>
#include <sdl/text.h>
#include <stdlib.h>

path_resolver_t resolver;

SDL_AppResult SDL_AppIterate(void *appstate) {
  appstate_t  *as  = (appstate_t *)appstate;
  const Uint64 now = SDL_GetTicks();

  while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
    as->last_step += STEP_RATE_IN_MILLISECONDS;
  }

  SDL_SetRenderDrawColor(as->renderer, 164, 116, 73, 255);
  SDL_RenderClear(as->renderer);

  /*as->map_viewport.w=as->window_width;
  as->map_viewport.h=as->window_height;*/
  SDL_Rect v = {(int)as->map_viewport.x, (int)as->map_viewport.y,
                (int)as->map_viewport.w, (int)as->map_viewport.h};
  SDL_SetRenderViewport(as->renderer, &v);

  render_map(as);

  if (as->display_grid) {
    print_grid(as);
  }

  SDL_SetRenderViewport(as->renderer, NULL);

  /*if (as->text) {
    SDL_FRect dest = {50.0f, 50.0f, as->text->w, as->text->h};
    SDL_RenderTexture(as->renderer, as->text->texture, NULL, &dest);
  }

  SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
  SDL_RenderRect(as->renderer, &as->map_viewport);*/

  for (unsigned int nb_players = 0;
       nb_players < as->engine.game.options->players; nb_players++) {
    render_banner(as->banners[nb_players], as->renderer);
  }
  SDL_RenderPresent(as->renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  create_path_resolver(&resolver);
  options_t options = parse_options(argc, argv);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return SDL_APP_FAILURE;
  }

  appstate_t *as = (appstate_t *)SDL_calloc(1, sizeof(appstate_t));
  if (!as) {
    return SDL_APP_FAILURE;
  }
  *appstate = as;

  return_code_t init = create_engine(&as->engine, options);

  if (init != 0) {
    return SDL_APP_FAILURE;
  }

  start_game(&as->engine);

  // pour resolve:
  char *path;

  if (!SDL_CreateWindowAndRenderer("Carcassonne Test", DEFAULT_WINDOW_WIDTH,
                                   DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE,
                                   &as->window, &as->renderer)) {
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
  as->display_grid = DISPLAY_GRID_BY_DEFAULT;

  as->banners = create_banner_for_each_player(as);

  get_current_actions(as);
  put_first_action_in_appstate(as);
  update_possible_places(as);

  synchronize_banners(as);
  as->last_step = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  appstate_t *as = (appstate_t *)appstate;
  switch (event->type) {
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      as->window_width   = (float)event->window.data1;
      as->window_height  = (float)event->window.data2;
      as->map_viewport.w = as->window_width;
      as->map_viewport.h = as->window_height;
      printf("Nouvelle taille de rendu en pixels : %d x %d\n",
             event->window.data1, event->window.data2);
      // printf("Viewport size : x = %f, y = %f, w = %f, h =
      // %f\n",as->map_viewport.x,as->map_viewport.y,as->map_viewport.w,as->map_viewport.h);
      center_camera_on_start(as);
      break;
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
    for (unsigned int nb_players = 0;
         nb_players < as->engine.game.options->players; nb_players++) {
      destroy_banner(as->banners[nb_players]);
    }
    SDL_free(as->banners);
    vector_free(&as->possibles_places);
    hashmap_free(&as->textures);
    SDL_free(as);
    TTF_Quit();
  }
}

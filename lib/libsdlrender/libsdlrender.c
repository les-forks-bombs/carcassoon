#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <assert.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/libsdlrender.h>
#include <libsdlrender/load.h>
#include <stdio.h>

#include "libai/mcts.h"
#include "libcarcassonne/enums.h"
#include "libcarcassonne/game.h"
#include "libsdlrender/action.h"
#include "libsdlrender/appstate.h"
#include "libsdlrender/banner.h"
#include "libsdlrender/camera.h"
#include "libsdlrender/events.h"
#include "libsdlrender/forward.h"
#include "libsdlrender/map.h"
#include "libutils/hashmap.h"
#include "libutils/lc.h"
#include "libutils/path.h"
#include "libutils/vector.h"

return_code_t run_sdl(engine_t* engine) {
  return_code_t   err;
  path_resolver_t resolver;

  err = create_path_resolver(&resolver);
  if (err != SUCCESS) {
    return err;
  }

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("error: Failed to initialize SDL\n");
    return ERROR;
  }

  if (!TTF_Init()) {
    printf("error: Failed to initialize SDL_TTF\n");
    return ERROR;
  }

  appstate_t state = {0};
  appstate_create(&state, engine);

  char* font =
      path_resolver_resolve(&state.resolver, "assets/fonts/Orange.ttf");
  state.font = TTF_OpenFont(font, 24);
  free(font);

  if (!SDL_CreateWindowAndRenderer("Carcassonne | Les Forks bombs",
                                   DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                   SDL_WINDOW_RESIZABLE, &state.window,
                                   &state.renderer)) {
    printf("error: Failed to create the SDL renderer\n");
    return ERROR;
  }

  char* assets = path_resolver_resolve(&state.resolver, "assets");
  char* img    = path_resolver_resolve(&state.resolver, "assets/img");
  load_textures(&state, img, assets);
  free(assets);
  free(img);

  SDL_SetRenderVSync(state.renderer, true);
  center_camera_on_start(&state);

  get_current_actions(&state);
  put_first_action_in_appstate(&state);
  update_possible_places(&state);

  synchronize_banners(&state);

  state.playing = true;

  SDL_Event event;
  while (state.playing) {

    if(game_get_current_player(&engine->game)->player_type==LIBCARCASSONNE_PLAYER_AI && !is_game_finished(&engine->game)){
      ai_play_turn(engine, 20);
      get_current_actions(&state);
    }else {
      while (SDL_PollEvent(&event)) {
        handle_app_event(&state, &event);
      }
    }


    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);

    SDL_Rect map_viewport = {
        (int)state.map_viewport.x, (int)state.map_viewport.y,
        (int)state.map_viewport.w, (int)state.map_viewport.h};

    SDL_SetRenderViewport(state.renderer, &map_viewport);
    render_map(&state);
    SDL_SetRenderViewport(state.renderer, NULL);

    for (unsigned int i = 0; i < state.engine->config.players; i++) {
      render_banner(&state, &state.banners[i]);
    }

    SDL_RenderPresent(state.renderer);
  }

  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);

  for (unsigned int i = 0; i < vector_size(&state.textures.meta.buckets); i++) {
    list_t* bucket = vector_nth(&state.textures.meta.buckets, i);

    for (list_node_t* node = bucket->head; node != NULL; node = node->next) {
      hashmap_node_t* hashmap = (hashmap_node_t*)&node->value;
      SDL_DestroyTexture(hashmap->value);
    }
  }

  for (unsigned int i = 0; i < state.engine->config.players; i++) {
    destroy_banner(&state.banners[i]);
  }

  hashmap_free(&state.textures);
  vector_free(&state.all_actions);
  vector_free(&state.possibles_places);

  TTF_CloseFont(state.font);
  TTF_Quit();

  return SUCCESS;
}

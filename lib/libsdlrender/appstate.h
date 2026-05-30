#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/camera.h>
#include <libutils/hashmap.h>

#include "libcarcassonne/macros.h"
#include "libsdlrender/banner.h"
#include "libsdlrender/forward.h"
#include "libutils/path.h"

typedef HashMap(char *, SDL_Texture *) textures_hashmap_t;

struct appstate {
  SDL_Window        *window;
  SDL_Renderer      *renderer;
  camera_t           camera;
  SDL_FRect          map_viewport;
  engine_t          *engine;
  textures_hashmap_t textures;
  vector2d_vector_t  possibles_places;
  bool               possible_meeples[9];
  float              window_width, window_height;
  TTF_Font          *font;
  banner_t           banners[LIBCARCASSONNE_MAX_PLAYERS];
  action_t          *current_action;
  action_vector_t    all_actions;
  bool               is_waiting_for_tile;
  unsigned int       action_index;
  bool               display_grid;

  bool playing;

  path_resolver_t resolver;
};

return_code_t appstate_create(appstate_t *state, engine_t *engine);

#endif

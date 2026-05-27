#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL3/SDL.h>
#include <libcarcassonne/engine.h>
#include <libutils/hashmap.h>
#include <sdl/forward.h>

#include "libcarcassonne/forward.h"

typedef HashMap(char *, SDL_Texture *) textures_hashmap_t;

struct appstate {
  SDL_Window        *window;
  SDL_Renderer      *renderer;
  camera_t          *camera;
  SDL_FRect          map_viewport;
  Uint64             last_step;
  text_object_t     *text;
  engine_t           engine;
  SDL_Texture       *temp_tex;
  textures_hashmap_t textures;
  vector2d_vector_t  possibles_places;
  bool              *possible_meeples;
  float              window_width, window_height;

  banner_t      **banners;
  action_t       *current_action;
  action_vector_t all_actions;
  bool            is_waiting_for_tile;
  int             action_index;
  bool display_grid;
};

#endif
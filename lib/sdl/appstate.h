#ifndef APPSTATE_H
#define APPSTATE_H

#include <sdl/banner.h>
#include <sdl/camera.h>
#include <libcarcassonne/engine.h>
#include <libutils/hashmap.h>

typedef HashMap(char *, SDL_Texture*) textures_hashmap_t;

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
  textures_hashmap_t textures;

  banner_t **banners;
} AppState;

#endif
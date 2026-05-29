#include <SDL3/SDL_stdinc.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/camera.h>
#include <libsdlrender/consts.h>
#include <libsdlrender/forward.h>
#include <unistd.h>

#include "libcarcassonne/enums.h"

return_code_t create_camera(camera_t *camera) {
  if (camera == NULL) {
    return NULL_POINTER;
  }

  camera->x    = 0.0F;
  camera->y    = 0.0F;
  camera->zoom = 1.0F;
  return SUCCESS;
}

void update_camera(appstate_t *state) {
  if (state->camera.x < 0) {
    state->camera.x = 0;
  }

  float max_x = (MAP_TABLE_SIZE * MAP_TILE_SIZE) -
                (state->window_width / state->camera.zoom);
  if (state->camera.x > max_x) {
    state->camera.x = max_x;
  }

  if (state->camera.y < 0) {
    state->camera.y = 0;
  }

  float max_y = (MAP_TABLE_SIZE * MAP_TILE_SIZE) -
                (state->window_height / state->camera.zoom);
  if (state->camera.y > max_y) {
    state->camera.y = max_y;
  }
}

void center_camera_on_start(appstate_t *state) {
  float world_center_x = 0.0F;
  float world_center_y = 0.0F;

  state->camera.x =
      world_center_x - ((state->map_viewport.w / 2.0F) / state->camera.zoom);
  state->camera.y =
      world_center_y - ((state->map_viewport.h / 2.0F) / state->camera.zoom);
}


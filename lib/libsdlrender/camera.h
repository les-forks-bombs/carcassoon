#pragma once

#include <SDL3/SDL_rect.h>
#include <libsdlrender/forward.h>

#include "libcarcassonne/enums.h"

struct camera {
  float x, y;
  float zoom;
};

return_code_t create_camera(camera_t *camera);
void          update_camera(appstate_t *state);
void          center_camera_on_start(appstate_t *state);

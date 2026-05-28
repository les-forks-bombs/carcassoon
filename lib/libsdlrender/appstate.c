#include <libsdlrender/appstate.h>
#include <stdio.h>
#include <string.h>

#include "libcarcassonne/enums.h"
#include "libsdlrender/banner.h"
#include "libsdlrender/camera.h"
#include "libsdlrender/consts.h"
#include "libsdlrender/forward.h"

return_code_t appstate_create(appstate_t* state, engine_t* engine) {
  memset(state, 0, sizeof(*state));

  if (create_path_resolver(&state->resolver) != SUCCESS) {
    return ERROR;
  }

  if (create_camera(&state->camera) != SUCCESS) {
    printf("error: Failed to create the camera\n");
    return ERROR;
  }

  state->engine         = engine;
  state->map_viewport.w = DEFAULT_WINDOW_WIDTH;
  state->map_viewport.h = DEFAULT_WINDOW_HEIGHT;
  hashmap_create(&state->textures, 256);

  for (unsigned i = 0; i < state->engine->config.players; i++) {
    create_banner(state, &state->banners[i], players_colors[i], i);
  }

  return SUCCESS;
}

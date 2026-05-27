#include <SDL3/SDL_stdinc.h>
#include <sdl/appstate.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/forward.h>

camera_t *create_camera(void) {
  camera_t *cam = SDL_calloc(1, sizeof(camera_t));
  if (cam) {
    cam->x    = 0.0F;
    cam->y    = 0.0F;
    cam->zoom = 1.0F;
  }
  return cam;
}

void update_camera(appstate_t *as) {
  camera_t *cam = as->camera;
  if (cam->x < 0) {
    cam->x = 0;
  }

  float max_x =
      (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (as->window_width / cam->zoom);
  if (cam->x > max_x) {
    cam->x = max_x;
  }

  if (cam->y < 0) {
    cam->y = 0;
  }

  float max_y =
      (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (as->window_height / cam->zoom);
  if (cam->y > max_y) {
    cam->y = max_y;
  }
}

void center_camera_on_start(appstate_t *as) {
  float world_center_x = 0.0F;
  float world_center_y = 0.0F;

  as->camera->x =
      world_center_x - ((as->map_viewport.w / 2.0F) / as->camera->zoom);
  as->camera->y =
      world_center_y - ((as->map_viewport.h / 2.0F) / as->camera->zoom);
}
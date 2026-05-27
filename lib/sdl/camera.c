#include <SDL3/SDL_stdinc.h>
#include <sdl/camera.h>
#include <sdl/consts.h>

camera_t *create_camera(void) {
  camera_t *cam = SDL_calloc(1, sizeof(camera_t));
  if (cam) {
    cam->x    = 0.0f;
    cam->y    = 0.0f;
    cam->zoom = 1.0f;
  }
  return cam;
}

void update_camera(camera_t *cam) {
  if (cam->x < 0) cam->x = 0;

  float max_x = (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (WINDOW_WIDTH / cam->zoom);
  if (cam->x > max_x) cam->x = max_x;

  if (cam->y < 0) cam->y = 0;

  float max_y = (MAP_TABLE_SIZE * MAP_TILE_SIZE) - (WINDOW_HEIGHT / cam->zoom);
  if (cam->y > max_y) cam->y = max_y;
}

void center_camera_on_start(camera_t *cam, SDL_Renderer *renderer) {
  /*float world_center_x = 0.0f;
  float world_center_y = 0.0f;

  cam->x = world_center_x - ((SDL_GetCurrentRenderOutputSize(renderer, &cam->y) / 2.0f) / cam->zoom);
  cam->y = world_center_y - ((SDL_GetCurrentRenderOutputSize(renderer, &cam->y) / 2.0f) / cam->zoom);*/
}
#include <SDL3/SDL_stdinc.h>
#include <sdl/camera.h>

camera_t* create_camera(void) {
  camera_t* cam = SDL_calloc(1, sizeof(camera_t));
  if (cam) {
    cam->x    = 0.0f;
    cam->y    = 0.0f;
    cam->zoom = 1.0f;
  }
  return cam;
}

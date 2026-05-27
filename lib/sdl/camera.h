#ifndef H_CAMERA
#define H_CAMERA

#include <SDL3/SDL_rect.h>
#include <sdl/forward.h>

struct camera {
  float x, y;
  float zoom;
};

camera_t *create_camera(void);
void      update_camera(appstate_t *as);
void      center_camera_on_start(appstate_t *as);

#endif

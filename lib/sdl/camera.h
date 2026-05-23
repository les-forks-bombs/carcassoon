#ifndef H_CAMERA
#define H_CAMERA

#include <SDL3/SDL_rect.h>
typedef struct {
  float x, y;
  float zoom;
} camera_t;

camera_t *create_camera(void);
void update_camera(camera_t *cam);
void center_camera_on_start(camera_t *cam, SDL_FRect *viewport);

#endif

#ifndef H_CAMERA
#define H_CAMERA

typedef struct {
    float x, y;
    float zoom;
} camera_t;

camera_t *create_camera(void);

#endif

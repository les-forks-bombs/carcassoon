#ifndef H_MAP
#define H_MAP

#include "camera.h"
#include "consts.h"
#include "tile_temp.h"

typedef struct {
    tile_temp_t *tiles[MAP_TABLE_SIZE*MAP_TABLE_SIZE];
} map_t;

map_t *create_map(void);

void render_map(map_t *map, SDL_Renderer* renderer, camera_t *cam);

void update_camera(camera_t *cam);

#endif

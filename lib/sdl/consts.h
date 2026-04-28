#ifndef H_LIBSDL_CONSTS
#define H_LIBSDL_CONSTS

#include <libcarcassonne/consts.h>

#define MAP_TILE_SIZE             64
#define MAP_TABLE_SIZE            (LIBCARCASSONNE_TILES_COUNT * 2 - 1)
#define WINDOW_WIDTH              1000
#define WINDOW_HEIGHT             700
#define WINDOW_RESIZABLE          0
#define ZOOM_MAX                  10
#define ZOOM_MIN                  0.1
#define STEP_RATE_IN_MILLISECONDS 16

#endif

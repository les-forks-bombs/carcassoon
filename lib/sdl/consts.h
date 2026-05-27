#ifndef H_LIBSDL_CONSTS
#define H_LIBSDL_CONSTS

#include <SDL3/SDL_pixels.h>
#include <libcarcassonne/consts.h>

#define MAP_TILE_SIZE             64
#define MAP_TABLE_SIZE            (LIBCARCASSONNE_TILES_COUNT * 2 - 1)
#define DEFAULT_WINDOW_WIDTH      1000
#define DEFAULT_WINDOW_HEIGHT     700
#define WINDOW_RESIZABLE          1
#define ZOOM_MAX                  10
#define ZOOM_MIN                  0.1
#define STEP_RATE_IN_MILLISECONDS 16
#define DISPLAY_GRID_BY_DEFAULT   0

static SDL_Color players_colors[] = {
    {255, 0, 0, 255}, {0, 0, 255, 255}, {255, 0, 255, 255}, {0, 255, 255, 255}};

#endif

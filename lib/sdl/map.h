#ifndef H_MAP
#define H_MAP

#include <libcarcassonne/forward.h>
#include <sdl/appstate.h>
#include <sdl/camera.h>
#include <sdl/consts.h>
#include <sdl/forward.h>
#include <sdl/meeple.h>

void                render_map(appstate_t *as);
static double       get_tile_angle(int orientation);
static SDL_Texture *get_tile_texture(appstate_t *as, const tile_t *tile);
static void draw_tile(appstate_t *as, const tile_t *tile, const SDL_FRect *dest,
                      double angle, Uint8 alpha);
static void draw_selection_border(SDL_Renderer    *renderer,
                                  const SDL_FRect *dest,appstate_t *as);
static void render_occupied_cell(appstate_t *as, placed_tile_t *ptt,
                                 const SDL_FRect *dest);
static void render_empty_cell(appstate_t *as, int table_x, int table_y,
                              const SDL_FRect *dest);
void        update_possible_places(appstate_t *as);

#endif

#ifndef SDL_MEEPLE_H
#define SDL_MEEPLE_H

#include <SDL3/SDL.h>
#include <libcarcassonne/forward.h>
#include <sdl/appstate.h>

#include "sdl/camera.h"

void render_possible_meeples(placed_tile_t *tile, AppState *as,
                             const SDL_FRect *tile_rect, double angle);
void render_placed_meeple(placed_tile_t *tile, AppState *as,
                          const SDL_FRect *tile_rect, double angle);
static SDL_FRect calc_meeple_rect(tile_slot_t slot, const SDL_FRect *tile_rect,
                                  double angle);
int get_meeple_slot_at_position(const placed_tile_t *tile,
                                const SDL_FRect *tile_rect, double angle,
                                const SDL_MouseButtonEvent *event);

void update_possible_meeples(AppState *as);

#endif
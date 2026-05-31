#ifndef SDL_MEEPLE_H
#define SDL_MEEPLE_H

#include <SDL3/SDL.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/appstate.h>
#include <libsdlrender/camera.h>

/// @brief Affiche les meeples plaçables sur la tuile en cours
/// @param tile_rect La position de la tuile en cours
/// @param angle L'angle de la tuile en cours
void render_possible_meeples(placed_tile_t *tile, appstate_t *as,
                             const SDL_FRect *tile_rect, double angle);
/// @brief Affiche les meeples placés sur une tuile
/// @param tile La tuile sur laquelle afficher le meeple
/// @param tile_rect La position de la tuile sur laquelle afficher le meeple
/// @param angle L'angle de la tuile sur laquelle placer le meeple
void render_placed_meeple(placed_tile_t *tile, appstate_t *as,
                          const SDL_FRect *tile_rect, double angle);
/// @param Met à jour la liste des meeples plaçables en synchronisant avec la
/// partie en cours
void update_possible_meeples(appstate_t *as);

#endif

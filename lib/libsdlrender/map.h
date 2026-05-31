#pragma once

#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/appstate.h>

/// @brief Met à jour l'affichage de la carte
void render_map(appstate_t *as);
/// @brief Met à jour les coordonnées des places possibles
void update_possible_places(appstate_t *as);

#pragma once

#include <SDL3/SDL_rect.h>
#include <libsdlrender/forward.h>

#include "libcarcassonne/enums.h"

/// @brief Représente une caméra pour l'affichage de la carte
struct camera {
  /// @brief La position de la caméra
  float x, y;
  float zoom;
};

/// @brief Permet de créer une instance d'une caméra
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
return_code_t create_camera(camera_t *camera);
/// @brief Met à jour l'affichage en fonction de la position de la caméra
/// @param state L'affichage de la partie en cours
void          update_camera(appstate_t *state);
/// @brief Centre la caméra autour de la tuile de départ
/// @param state L'affichage de la partie en cours
void          center_camera_on_start(appstate_t *state);

#ifndef BANNER_H
#define BANNER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <libcarcassonne/forward.h>
#include <libsdlrender/text.h>
#include <stdbool.h>

#include "libsdlrender/forward.h"

/// @brief Représente une bannière
struct banner {
  /// @brief L'état de la bannière
  bool is_open;
  /// @brief La couleur de la bannière
  SDL_Color color;
  /// @brief La position de la bannière
  SDL_FRect area;
  /// @brief Le score affiché sur la bannière
  text_object_t score_object;
  /// @brief Le nombre de meeples affiché sur la bannière
  text_object_t meeple_count_object;
};

/// @brief Permet de créer une bannière
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
/// @param as L'appstate dans laquelle créer la bannière
/// @param color La couleur de la bannière
/// @param nb Le numéro du joueur de la bannière
return_code_t create_banner(appstate_t *as, banner_t *banner, SDL_Color color,
                            unsigned int nb);
/// @brief Met à jour l'affichage de la bannière
/// @param banner La bannière à afficher
/// @param as L'appstate sur laquelle afficher la bannière
void render_banner(appstate_t *as, banner_t *banner);
/// @brief Change l'état de la bannière
/// @details Ouvre la bannière si elle est fermée et inversement
/// @param banner La bannière à ouvrir/fermer
void toggle_banner(banner_t *banner);
/// @brief Permet de désallouer une instance de bannière
/// @param banner La bannière à détruire
void destroy_banner(banner_t *banner);
/// @brief Met à jour les données des bannières avec les données de la partie
/// @param as L'appstate dans laquelle a lieu la partie
void synchronize_banners(appstate_t *as);

#endif

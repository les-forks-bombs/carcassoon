#ifndef APPSTATE_H
#define APPSTATE_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libsdlrender/camera.h>
#include <libutils/hashmap.h>

#include "libcarcassonne/macros.h"
#include "libsdlrender/banner.h"
#include "libsdlrender/forward.h"
#include "libutils/path.h"

typedef HashMap(char *, SDL_Texture *) textures_hashmap_t;

/// @brief Représente les paramètres persistants de l'affichage
struct appstate {
  /// @brief La fenêtre SDL
  SDL_Window        *window;
  /// @brief Le renderer SDL
  SDL_Renderer      *renderer;
  /// @brief L'instance de la caméra
  camera_t           camera;
  /// @brief Le rectangle dans lequel est affichée la carte
  SDL_FRect          map_viewport;
  /// @brief Le moteur de la partie en cours
  engine_t          *engine;
  /// @brief Le dictionnaire contenant toutes les textures pour le rendu
  textures_hashmap_t textures;
  /// @brief La liste des coordonnées possibles pour placer la prochaine tuile
  vector2d_vector_t  possibles_places;
  /// @brief La liste des groupes de meeples plaçables sur la tuile en cours
  bool               possible_meeples[9];
  /// @brief Les dimensions de la fenêtre SDL
  float              window_width, window_height;
  /// @brief La police d'écriture
  TTF_Font          *font;
  /// @brief Une liste de bannières, une par joueur
  banner_t           banners[LIBCARCASSONNE_MAX_PLAYERS];
  /// @brief L'action sélectionnée
  action_t          *current_action;
  /// @brief La liste de toutes les actions possibles
  action_vector_t    all_actions;
  /// @brief Le type de la prochaine action
  bool               is_waiting_for_tile;
  /// @brief L'index de current_action dans all_actions
  unsigned int       action_index;
  /// @brief Le paramètre pour l'affichage de la grille
  bool               display_grid;
  /// @brief L'état de la partie en cours
  bool playing;
  /// @brief Le chemin actuel de l'exécutable
  path_resolver_t resolver;
};

/// @brief Permet de créer une instance d'appstate
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
return_code_t appstate_create(appstate_t *state, engine_t *engine);

#endif

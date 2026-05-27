#pragma once

#include "libcarcassonne/enums.h"
#include "libcarcassonne/forward.h"

/// @brief Action de placement de tuile
struct action_place_tile {
  /// @brief Coordonnée en abscisse
  int x;
  /// @brief Coordonné en ordonnée
  int y;
  /// @brief Le prototype de tuile à placer
  const tile_t *tile;
  /// @brief L'orientation de la tuile à placer
  tile_orientation_t orientation;
};

/// @brief Action de pose d'un meeple
struct action_place_meeple {
  /// @brief Coordonnée en abscisse
  int x;
  /// @brief Coordonné en ordonnée
  int y;
  /// @brief La tuile sur laquelle placer le meeple
  placed_tile_t *tile;
  /// @brief Le sous-groupe auquel le meeple doit appartenir
  int part_group;
  /// @brief Le type de meeple à placer
  meeple_type_t meeple_type;
};

/// @brief Union contenant les actions réalisables
union action_order {
  action_place_tile_t   place_tile;
  action_place_meeple_t place_meeple;
};

/// @brief Action à réaliser
struct action {
  /// @brief Le type d'action à réaliser
  action_type_t type;
  /// @brief La manière de réaliser l'action
  action_order_t order;
};

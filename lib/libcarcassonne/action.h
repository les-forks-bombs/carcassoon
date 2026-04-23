#ifndef H_LIBCARCASSONNE_ACTION
#define H_LIBCARCASSONNE_ACTION

#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>

/// @brief Représente les différents types d'action possible
typedef enum action_type {
  /// @brief Mettre fin au tour du joueur courant
  LIBCARCASSONNE_ACTION_END_PLAYER_TURN = -1,
  /// @brief Poser une tuile sur la plateau
  LIBCARCASSONNE_ACTION_PLACE_TILE = 0,
  /// @brief Poser un meeple
  LIBCARCASSONNE_ACTION_PLACE_MEEPLE = 1,
} action_type_t;

/// @brief Action de placement de tuile
typedef struct action_place_tile {
  /// @brief Coordonnée en abscisse
  int x;
  /// @brief Coordonné en ordonnée
  int y;
  /// @brief Le prototype de tuile à placer
  tile_t *tile;
  /// @brief L'orientation de la tuile à placer
  tile_orientation_t orientation;
} action_place_tile_t;

/// @brief Action de pose d'un meeple
typedef struct action_place_meeple {
  /// @brief Coordonnée en abscisse
  int x;
  /// @brief Coordonné en ordonnée
  int y;
  /// @brief La tuile sur laquelle placer le meeple
  placed_tile_t *tile;
  /// @brief Le sous-groupe auquel le meeple doit appartenir
  tile_part_group_t part_group;
} action_place_meeple_t;

/// @brief Union contenant les actions réalisables
typedef union action_order {
  action_place_tile_t   place_tile;
  action_place_meeple_t place_meeple;
} action_order_t;

/// @brief Action à réaliser
typedef struct action {
  /// @brief Le type d'action à réaliser
  action_type_t type;
  /// @brief La manière de réaliser l'action
  action_order_t order;
} action_t;

#endif
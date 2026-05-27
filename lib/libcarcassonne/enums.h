#pragma once

/// @brief Les sous-groupes possibles [0-7]
enum tile_part_group {
  LIBCARCASSONNE_TILE_PART_A = 0,
  LIBCARCASSONNE_TILE_PART_B = 1,
  LIBCARCASSONNE_TILE_PART_C = 2,
  LIBCARCASSONNE_TILE_PART_D = 3,
  LIBCARCASSONNE_TILE_PART_E = 4,
  LIBCARCASSONNE_TILE_PART_F = 5,
  LIBCARCASSONNE_TILE_PART_G = 6,
  LIBCARCASSONNE_TILE_PART_H = 7,
  LIBCARCASSONNE_TILE_PART_I = 8
};

/// @brief Les orientations possibles pour les tiles
/// @remark Pour avoir l'opposé on a (n + 2) % 4
/// @remark Pour avoir le suivant (sens des aiguilles d'une montre on à) (n + 1)
/// % 4
enum tile_orientation {
  LIBCARCASSONNE_TILE_ORIENTATION_NORTH = 0,
  LIBCARCASSONNE_TILE_ORIENTATION_EAST  = 1,
  LIBCARCASSONNE_TILE_ORIENTATION_SOUTH = 2,
  LIBCARCASSONNE_TILE_ORIENTATION_WEST  = 3
};

/// @brief Les différents types de sous-tiles
enum tile_part_kind {
  LIBCARCASSONNE_TILE_PART_VILLAGE = 0,
  LIBCARCASSONNE_TILE_PART_FIELD   = 1,
  LIBCARCASSONNE_TILE_PART_ROAD    = 2,
  LIBCARCASSONNE_TILE_PART_WALL    = 3,
  LIBCARCASSONNE_TILE_PART_ABBEY   = 4,
  LIBCARCASSONNE_TILE_PART_TOWN    = 5
};

/// @brief Représente les différents types d'action possible
typedef enum action_type {
  LIBCARCASSONNE_ACTION_NONE = 0,
  /// @brief Poser une tuile sur la plateau
  LIBCARCASSONNE_ACTION_PLACE_TILE = 1,
  /// @brief Poser un meeple
  LIBCARCASSONNE_ACTION_PLACE_MEEPLE = 2,
} action_type_t;

enum return_code {
  SUCCESS              = 0,
  ERROR                = 1,
  NOT_FREE             = 2,
  OUT_OF_BOUNDS        = 3,
  NO_TILE              = 4,
  INVALID_PLACEMENT    = 5,
  ALREADY_ALLOCATED    = 6,
  INVALID_ACTION       = 7,
  NULL_POINTER         = 8,
  NO_MORE_PLAYER       = 9,
  PLAYER_NOT_CALLED    = 10,
  NO_PROGRESS          = 11,
  REMOVE_TILE_NOT_VOID = 12,
  GAME_NOT_STARTED     = 13,
  GAME_FINISHED        = 14,
};

enum player_type {
  LIBCARCASSONNE_PLAYER_AI    = 0,
  LIBCARCASSONNE_PLAYER_HUMAN = 1
};
typedef enum player_type player_type_t;

typedef enum meeple_type {
  NONE  = -1,
  BASIC = 0,
  LARGE = 1,
  ABBOT = 2
} meeple_type_t;

typedef enum options_mode {
  CARCASSONNE_MODE_SDL = 0,
  CARCASSONNE_MODE_CLI = 1,
} options_mode_t;

/// @brief État de la partie
typedef enum {
  GAME_STATE_NOT_STARTED = 0,  // Partie non démarrée
  GAME_STATE_PLAYING     = 1,  // Partie en cours
  GAME_STATE_FINISHED    = 2,  // Partie terminée
} game_state_t;

typedef enum return_code      return_code_t;
typedef enum tile_part_kind   tile_part_type_t;
typedef enum tile_part_group  tile_part_group_t;
typedef enum tile_orientation tile_orientation_t;
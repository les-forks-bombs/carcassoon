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
    /// @brief Orientation Nord
  LIBCARCASSONNE_TILE_ORIENTATION_NORTH = 0,
    /// @brief Orientation Est
  LIBCARCASSONNE_TILE_ORIENTATION_EAST  = 1,
    /// @brief Orientation Sud
  LIBCARCASSONNE_TILE_ORIENTATION_SOUTH = 2,
    /// @brief Orientation Ouest
  LIBCARCASSONNE_TILE_ORIENTATION_WEST  = 3
};

/// @brief Les différents types de sous-tiles
enum tile_part_kind {
    /// @brief Représente un village
  LIBCARCASSONNE_TILE_PART_VILLAGE = 0,
    /// @brief Représente un champ
  LIBCARCASSONNE_TILE_PART_FIELD   = 1,
    /// @brief Représente une route
  LIBCARCASSONNE_TILE_PART_ROAD    = 2,
    /// @brief Représente un mur
  LIBCARCASSONNE_TILE_PART_WALL    = 3,
    /// @brief Représente une abbaye
  LIBCARCASSONNE_TILE_PART_ABBEY   = 4,
    /// @brief Représente une ville
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

/// @brief Code de retour permettant d'identifier et de comprendre le résultat
/// d'une fonction
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

/// @brief Représente les différents types de joueurs
enum player_type {
  /// @brief Représente un joueur IA
  LIBCARCASSONNE_PLAYER_AI = 0,
  /// @brief Représente un joueur humain
  LIBCARCASSONNE_PLAYER_HUMAN = 1
};
/// @brief Réprésente un joueur dans la partie
typedef enum player_type player_type_t;

/// @brief Représente les types de meeple
typedef enum meeple_type {
  /// @brief Représente la non-pose de meeple
  NONE = -1,
  /// @brief Représente le meeple de base
  BASIC = 0,
  /// @brief Représente le grand meeple
  LARGE = 1,
  /// @brief Représente l'abbé
  ABBOT = 2
} meeple_type_t;

typedef enum options_mode {
  CARCASSONNE_MODE_SDL = 0,
  CARCASSONNE_MODE_CLI = 1,
} options_mode_t;

/// @brief État de la partie
typedef enum {
  /// @brief Partie non démarrée
  GAME_STATE_NOT_STARTED = 0,
  /// @brief Partie en cours
  GAME_STATE_PLAYING = 1,
  /// @brief Partie terminée
  GAME_STATE_FINISHED = 2,
} game_state_t;

typedef enum return_code      return_code_t;
/// @brief Les différents types de sous-tiles
typedef enum tile_part_kind   tile_part_type_t;
/// @brief Les sous-groupes possibles [0-7]
typedef enum tile_part_group  tile_part_group_t;

/// @brief Les orientations possibles pour les tiles
/// @remark Pour avoir l'opposé on a (n + 2) % 4
/// @remark Pour avoir le suivant (sens des aiguilles d'une montre on à) (n + 1)
/// % 4
typedef enum tile_orientation tile_orientation_t;
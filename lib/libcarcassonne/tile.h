#pragma once

#include <libcarcassonne/consts.h>
#include <stdbool.h>

/// @brief Les différents types de sous-tiles
enum tile_part_kind {
  LIBCARCASSONNE_TILE_PART_VILLAGE = 0,
  LIBCARCASSONNE_TILE_PART_FIELD   = 1,
  LIBCARCASSONNE_TILE_PART_ROAD    = 2,
  LIBCARCASSONNE_TILE_PART_WALL    = 3,
  LIBCARCASSONNE_TILE_PART_ABBEY   = 4,
  LIBCARCASSONNE_TILE_PART_TOWN    = 5
};

/// @brief Les sous-groupes possibles [0-7]
enum tile_part_group {
  LIBCARCASSONNE_TILE_PART_A = 0,
  LIBCARCASSONNE_TILE_PART_B = 1,
  LIBCARCASSONNE_TILE_PART_C = 2,
  LIBCARCASSONNE_TILE_PART_D = 3,
  LIBCARCASSONNE_TILE_PART_E = 4,
  LIBCARCASSONNE_TILE_PART_F = 5,
  LIBCARCASSONNE_TILE_PART_G = 6,
  LIBCARCASSONNE_TILE_PART_H = 7
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

typedef enum tile_part_kind   tile_part_type_t;
typedef enum tile_part_group  tile_part_group_t;
typedef enum tile_orientation tile_orientation_t;

/// @brief Une tile est un élément de jeu de carcassonne
typedef struct tile {
  /// @brief Famille de la tile
  char family[5];
  /// @brief Parties de la tile
  tile_part_type_t parts[9];
  /// @brief Spécifie si la tile est un blason
  bool blason;
  /// @brief Spécifie le nombre de tiles présentes
  unsigned int amount;
  /// @brief Définition des groupes a l'intérieur d'une tile
  tile_part_group_t parts_groups[9];
  /// @brief nom de l'image correspondante
  char* texture;
} tile_t;

typedef struct extension_tiles {
  const unsigned int size;
  const tile_t*      tiles;
} extension_tiles_t;

/// @brief Récupère une famille de la tuile en fonction de l'orientation et de
/// la face de connexion
/// @param tile Le modèle de tuile à utiliser
/// @param orientation L'orientation de la tuile
/// @param connexion_face La face de connexion
/// @return La famille de la face de connexion
/// @related tile_t
tile_part_type_t   tile_get_family_face(const tile_t*      tile,
                                        tile_orientation_t orientation,
                                        tile_orientation_t connexion_face);
tile_orientation_t tile_orientation_invert(tile_orientation_t orientation);

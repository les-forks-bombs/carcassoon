#ifndef H_LIBCARCASSONNE_TILE
#define H_LIBCARCASSONNE_TILE

#include <libcarcassonne/meeple.h>
#include <stdbool.h>

#define LIBCARCASSONNE_TILE_PART_VILLAGE 0
#define LIBCARCASSONNE_TILE_PART_FIELD   1
#define LIBCARCASSONNE_TILE_PART_ROAD    2
#define LIBCARCASSONNE_TILE_PART_WALL    3
#define LIBCARCASSONNE_TILE_PART_ABBEY   4
#define LIBCARCASSONNE_TILE_PART_TOWN    5

#define LIBCARCASSONNE_TILE_PART_A 1 << 0
#define LIBCARCASSONNE_TILE_PART_B 1 << 1
#define LIBCARCASSONNE_TILE_PART_C 1 << 2
#define LIBCARCASSONNE_TILE_PART_D 1 << 3
#define LIBCARCASSONNE_TILE_PART_E 1 << 4
#define LIBCARCASSONNE_TILE_PART_F 1 << 5
#define LIBCARCASSONNE_TILE_PART_G 1 << 6
#define LIBCARCASSONNE_TILE_PART_H 1 << 7

#define LIBCARCASSONNE_TILE_PART_WILDCARD                       \
  LIBCARCASSONNE_TILE_PART_A                                    \
  | LIBCARCASSONNE_TILE_PART_B | LIBCARCASSONNE_TILE_PART_C |   \
      LIBCARCASSONNE_TILE_PART_D | LIBCARCASSONNE_TILE_PART_E | \
      LIBCARCASSONNE_TILE_PART_F | LIBCARCASSONNE_TILE_PART_G | \
      LIBCARCASSONNE_TILE_PART_H

#define LIBCARCASSONNE_TILE_ORIENTATION_NORTH 0
#define LIBCARCASSONNE_TILE_ORIENTATION_EAST  1
#define LIBCARCASSONNE_TILE_ORIENTATION_SOUTH 2
#define LIBCARCASSONNE_TILE_ORIENTATION_WEST  3

typedef char tile_part_type_t;
typedef char tile_part_group_t;
typedef char tile_orientation_t;

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
  char *texture;
} tile_t;

typedef struct placed_tile {
  tile_t*            parent;
  meeple_t*          meeple[9];
  tile_orientation_t orientation;
} placed_tile_t;

/// @brief Récupère une famille de la tuile en fonction de l'orientation et de
/// la face de connexion
/// @param tile Le modèle de tuile à utiliser
/// @param orientation L'orientation de la tuile
/// @param connexion_face La face de connexion
/// @return La famille de la face de connexion
/// @related tile_t
char tile_get_family_face(tile_t* tile, tile_orientation_t orientation,
                          tile_orientation_t connexion_face);

#endif

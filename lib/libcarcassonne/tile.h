#ifndef H_LIBCARCASSONNE_TILE
#define H_LIBCARCASSONNE_TILE

#include <libcarcassonne/consts.h>
#include <libcarcassonne/meeple.h>
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
} tile_t;

/// @brief Information des groupes de la sous-tile
typedef struct placed_tile_group {
  /// @brief Groupes adjacents qui sont enfants
  struct placed_tile_group* neighbors[4];
  /// @brief Référence vers la tile représentant le groupe
  struct placed_tile* tile;
  /// @brief Référence vers l'éventuel meeple placé dans le groupe
  meeple_t* meeple;
} placed_tile_group_t;

/// @brief Représentation d'une tile qui a été placée
typedef struct placed_tile {
  /// @brief Pointeur vers la définition de la tile
  tile_t* parent;
  /// @brief Groupes présents a l'intérieur de la tile
  /// @remark On peut au maximum avoir 9 groupes dans une tile
  placed_tile_group_t* groups[9];
  /// @brief Orientation de placement de la tile
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

return_code_t placed_tile_create(placed_tile_t* placed_tile, tile_t* parent,
                                 tile_orientation_t orientation);
void          placed_tile_destroy(placed_tile_t*);

tile_orientation_t tile_orientation_invert(tile_orientation_t orientation);

tile_part_type_t tile_orientation_face(placed_tile_t*     placed_tile,
                                       tile_orientation_t orientation);

placed_tile_group_t* tile_orientation_group(placed_tile_t*     placed_tile,
                                            tile_orientation_t orientation);

#endif
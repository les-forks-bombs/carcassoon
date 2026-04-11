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

#define LIBCARCASSONNE_TILE_PART_A 0
#define LIBCARCASSONNE_TILE_PART_B 1
#define LIBCARCASSONNE_TILE_PART_C 2
#define LIBCARCASSONNE_TILE_PART_D 3
#define LIBCARCASSONNE_TILE_PART_E 4
#define LIBCARCASSONNE_TILE_PART_F 5
#define LIBCARCASSONNE_TILE_PART_G 6
#define LIBCARCASSONNE_TILE_PART_H 7

#define LIBCARCASSONNE_TILE_ORIENTATION_NORTH 0
#define LIBCARCASSONNE_TILE_ORIENTATION_EAST  1
#define LIBCARCASSONNE_TILE_ORIENTATION_SOUTH 2
#define LIBCARCASSONNE_TILE_ORIENTATION_WEST  3

// note: south = north + 2
//       west  = east + 2
//       north = south + 2 % 4
//       east  = west + 2 % 4

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
} tile_t;


/// @brief Information des groupes de la sous-tile
typedef struct placed_tile_group {
  /// @brief Référence vers le groupe parent du groupe courrant
  /// @remark si null, alors le groupe actuel est une racine
  struct placed_tile_group* parent;
  /// @brief Groupes adjacents qui sont enfants
  struct placed_tile_group* child[4];
  /// @brief Référence vers la tile représentant le groupe
  struct placed_tile* tile;
  /// @brief Référence vers l'éventuel meeple placé dans le groupe
  meeple_t* meeple;
} placed_tile_group_t;

/// @brief Représentation d'une tile qui a été placée
typedef struct placed_tile {
  /// @brief Pointeur vers la définition de la tile
  tile_t*              parent;
  /// @brief Groupes présents a l'intérieur de la tile
  /// @remark On peut au maximum avoir 9 groupes dans une tile
  placed_tile_group_t* groups[9];
  /// @brief Orientation de placement de la tile
  tile_orientation_t   orientation;
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


tile_orientation_t tile_orientation_invert(
  tile_orientation_t orientation
);

tile_part_type_t tile_orientation_face(
  placed_tile_t* placed_tile,
  tile_orientation_t orientation
);

placed_tile_group_t* tile_orientation_group(
  placed_tile_t* placed_tile,
  tile_orientation_t orientation
);

#endif
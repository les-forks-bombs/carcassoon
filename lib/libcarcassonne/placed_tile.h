#include <libcarcassonne/meeple.h>

#ifndef H_LIBCARCASSONNE_PLACED_TILE
#define H_LIBCARCASSONNE_PLACED_TILE

/// @brief Information des groupes de la sous-tile
typedef struct placed_tile_group {
  /// @brief Référence vers la tile représentant le groupe
  struct placed_tile* tile;
  /// @brief Référence vers l'éventuel meeple placé dans le groupe
  meeple_t* meeple;

  struct placed_tile_group *right, *left, *parent;
} placed_tile_group_t;

typedef struct placed_face_groups {
  placed_tile_group_t* face[3];
} placed_face_groups_t;

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

return_code_t placed_tile_create(placed_tile_t* placed_tile, tile_t* parent,
                                 tile_orientation_t orientation);
void          placed_tile_destroy(placed_tile_t*);
return_code_t placed_tile_get_face(placed_face_groups_t* ret,
                                   placed_tile_t*        tile,
                                   tile_orientation_t    connexion_face);
void          placed_tile_group_access(placed_tile_group_t* node);
placed_tile_group_t* placed_tile_group_find_root(placed_tile_group_t* node);
void placed_tile_group_path_aggregate(placed_tile_group_t* node);
void placed_tile_group_cut(placed_tile_group_t* node);
void placed_tile_group_link(placed_tile_group_t* a, placed_tile_group_t* b);
void placed_tile_group_splay(placed_tile_group_t* node);
bool placed_tile_group_splay_is_root(placed_tile_group_t* node);
#endif
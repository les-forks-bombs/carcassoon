#pragma once
#include <libcarcassonne/meeple.h>

/// @brief Information des groupes de la sous-tile
typedef struct placed_tile_group {
  /// @brief Référence vers la tile représentant le groupe
  const struct placed_tile* tile;
  /// @brief Référence vers l'éventuel meeple placé dans le groupe
  meeple_t* meeple;

  /**
   * @brief Elements du L/C tree
   */
  struct placed_tile_group *right, *left, *parent;

  /**
   * @brief Tableau des enfants au total
   * Utilisé pour le parcours "simple" depuis la racine
   */
  struct placed_tile_group* weak_childs[3 * 4];
  /**
   * @brief Nombre d'enfants dans "weak_childs"
   */
  unsigned int weak_childs_count;

  /**
   * @brief
   *
   */
  bool flipped_delta;

  /**
   * @brief Le nombre de faces a remplir pour que le groupe soit completé
   * @remark Si ==0, alors le groupe est complété
   */
  unsigned int groupe_open_slots;

  /**
   * @brief Le nombre de tiles qui font partie du groupe
   */
  unsigned int groupe_taille;

  /**
   * @brief Le nombre de faces a remplir pour que le groupe soit completé
   * @remark Si ==0, alors le groupe est complété
   */
  unsigned int local_open_slots;
} placed_tile_group_t;

typedef struct placed_face_groups {
  placed_tile_group_t* face[3];
} placed_face_groups_t;

/// @brief Représentation d'une tile qui a été placée
typedef struct placed_tile {
  /// @brief Pointeur vers la définition de la tile
  const tile_t* parent;
  /// @brief Groupes présents a l'intérieur de la tile
  /// @remark On peut au maximum avoir 9 groupes dans une tile
  placed_tile_group_t* groups[9];
  /// @brief Orientation de placement de la tile
  tile_orientation_t orientation;
} placed_tile_t;

return_code_t        placed_tile_create(placed_tile_t*     placed_tile,
                                        const tile_t*      parent,
                                        tile_orientation_t orientation);
void                 placed_tile_destroy(placed_tile_t*);
return_code_t        placed_tile_get_face(placed_face_groups_t* ret,
                                          const placed_tile_t*  tile,
                                          tile_orientation_t    connexion_face);
void                 placed_tile_group_access(placed_tile_group_t* node);
placed_tile_group_t* placed_tile_group_find_root(placed_tile_group_t* node);
void placed_tile_group_path_aggregate(placed_tile_group_t* node);
void placed_tile_group_cut(placed_tile_group_t* node);
void placed_tile_group_link(placed_tile_group_t* a, placed_tile_group_t* b);
void placed_tile_group_splay(placed_tile_group_t* node);
bool placed_tile_group_splay_is_root(placed_tile_group_t* node);

void placed_tile_group_weak_childs_add(placed_tile_group_t* node,
                                       placed_tile_group_t* el);
void placed_tile_group_weak_childs_rem(placed_tile_group_t* node,
                                       placed_tile_group_t* el);
void placed_tile_group_push_down(placed_tile_group_t* node);
void placed_tile_group_makeroot(placed_tile_group_t* node);

bool placed_tile_group_complete(placed_tile_group_t* group);

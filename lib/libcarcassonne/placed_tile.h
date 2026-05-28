#pragma once
#include <libcarcassonne/forward.h>
#include <libutils/vector.h>

/// @brief Information des groupes de la sous-tile
struct placed_tile_group {
  /// @brief Référence vers la tile représentant le groupe
  const placed_tile_t *tile;
  /// @brief Référence vers l'éventuel meeple placé dans le groupe
  meeple_t *meeple;

  /**
   * @brief Tableau des enfants au total
   * Utilisé pour le parcours "simple" depuis la racine
   */
  placed_tile_group_neighbors_t neighbors;

  /// @brief Nombre d'emplacements ouverts du groupe
  unsigned int open_slots;

  /// @brief Identifiant de parcours
  int marker;

  /// @brief Type de structure
  tile_part_type_t type;
};

/// @brief Représentation d'une tile qui a été placée
struct placed_tile {
  /// @brief Pointeur vers la définition de la tile
  const tile_t *parent;
  /// @brief Groupes présents a l'intérieur de la tile
  /// @remark On peut au maximum avoir 9 groupes dans une tile
  placed_tile_group_t *groups[PLACED_TILE_GROUP_NUMBER];
  /// @brief Orientation de placement de la tile
  tile_orientation_t orientation;
  /// @brief Composante en x
  int x;
  /// @brief Composante en y
  int y;
};

/// @brief Représente le calculs des scores pour un groupe de tuiles
struct placed_tile_group_eval_points {
  /// @brief Le nombre de points à attribuer
  unsigned int points;
  /// @brief Les meeples concernés par l'évaluation
  meeple_vector_t meeples;
};

return_code_t placed_tile_create(placed_tile_t     *placed_tile,
                                 const tile_t      *parent,
                                 tile_orientation_t orientation, int x, int y);
void          placed_tile_destroy(placed_tile_t          */*placed_tile*/);

void placed_tile_group_create(placed_tile_group_t **group, meeple_t *meeple,
                              const placed_tile_t *tile);
void placed_tile_group_destory(placed_tile_group_t *group);

/**
 * @brief Crée un lien entre deux groupes
 *
 * @param a Le groupe a relier a "b"
 * @param b Le groupe a relier a "a"
 */
bool placed_tile_group_link(placed_tile_group_t *a, placed_tile_group_t *b);

/**
 * @brief Coupe le lien entre deux groupes
 *
 * @param a Le noeud "b" a couper de "a"
 * @param b Le noeud "a" a couper de "b"
 */
void placed_tile_group_cut(placed_tile_group_t *a, placed_tile_group_t *b);

/**
 * @brief Détermine si un groupe est complété (plus de faces ouvertes)
 *
 * @param group Le groupe a vérifier
 * @return true Toutes les faces sont remplies (ville fermée ou route completée)
 * @return false Il y a au moins une face ouverte
 */
bool placed_tile_group_complete(game_t *game, placed_tile_group_t *group);

/**
 * @brief Détermine si deux tiles sont connectées
 *
 * @param a Le noeud "b"
 * @param b Le noeud "a"
 * @return true Les deux tiles sont dans le meme groupe
 * @return false Les deux tiles ne sont pas dans le meme groupe
 */
bool placed_tile_group_connected(placed_tile_group_t *a,
                                 placed_tile_group_t *b);

/**
 * @brief Évalue le score d'un groupe
 *
 * @param game La partie en cours
 * @param group Le groupe évaluer
 * @param is_completed Si le groupe est complété ou non
 * @return Les résultats de l'évaluation des points
 */
placed_tile_group_eval_points_t placed_tile_group_eval_points(
    game_t *game, placed_tile_group_t *group, bool is_completed);

/// @brief Cherche et évalue le score des abbaye autour d'une tuile
/// @param game La partie en cours
/// @param placed_tile La tuile autour de laquelle on cherche
/// @return Les résultats de l'évaluation des abbayes
placed_tile_group_eval_points_vector_t check_for_abbey_completion(
    game_t *game, placed_tile_t *placed_tile);

/// @brief Évalue le score d'une abbaye
/// @param game La partie en cours
/// @param group Le groupe évaluer
/// @return Les résultats de l'évaluation de l'abbaye
placed_tile_group_eval_points_t eval_abbey(game_t              *game,
                                           placed_tile_group_t *group);
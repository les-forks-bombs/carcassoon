#pragma once

#include <libcarcassonne/extension.h>
#include <libcarcassonne/libcarcassonne.h>

struct meeple_place_hook_state {
  int               x, y;
  tile_part_group_t group;
  meeple_type_t     meeple_type;
};

struct tile_place_hook_state {
  int x, y;
};

struct rendre_meeple_hook_state {
  meeple_vector_t* meeples;
};

/// @brief Informations pour restaurer un meeple retiré
struct removed_meeple {
  int           x;       ///< Coordonnée X de la tuile
  int           y;       ///< Coordonnée Y de la tuile
  int           group;   ///< Groupe du meeple
  meeple_type_t type;    ///< Type du meeple
  player_t*     player;  ///< Joueur propriétaire
};

struct end_game_hook_state {
  /// @brief Score avant calcul final
  unsigned int saved_scores[LIBCARCASSONNE_MAX_PLAYERS];
  /// @brief Meeples retirés pour éviter les doublons (avec infos pour
  /// restauration)
  end_game_removed_meeples_vector_t removed_meeples;
};

/// @brief État sauvegardé pour un groupe complet avec meeples
/// (give_back_meeples)
struct give_back_scored_group {
  unsigned int                      points;
  bool                              player_won[LIBCARCASSONNE_MAX_PLAYERS];
  end_game_removed_meeples_vector_t meeples;
};

/**
 * @brief Définis le hook pour placer une tile
 *
 */
LIBCARCASSONNE_HOOK_HEADER(tile_place)

/**
 * @brief Définis le hook pour placer un meeple
 *
 */
LIBCARCASSONNE_HOOK_HEADER(meeple_place)

/**
 * @brief Hook de jeu qui restitue les meeples dans des groupes terminés (ex.
 * abbaye, route ou ville)
 *
 */
LIBCARCASSONNE_HOOK_HEADER(give_back_meeples)

/**
 * @brief Change le player courrant au prochain
 *
 */
LIBCARCASSONNE_HOOK_HEADER(next_player)

/**
 * @brief Vérifie si la partie est terminée
 *
 */
LIBCARCASSONNE_HOOK_HEADER(end_game)

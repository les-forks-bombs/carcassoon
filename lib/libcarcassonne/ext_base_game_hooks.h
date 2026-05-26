#pragma once

#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/tile.h>

#include "libcarcassonne/forward.h"

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
  int x;                ///< Coordonnée X de la tuile
  int y;                ///< Coordonnée Y de la tuile
  int group;            ///< Groupe du meeple
  meeple_type_t type;   ///< Type du meeple
  player_t *player;     ///< Joueur propriétaire
};

/// @brief Vecteur de meeples retirés pour end_game
ForwardDefinition(struct, removed_meeple);
typedef Vector(removed_meeple_t) end_game_removed_meeples_vector_t;

struct end_game_hook_state {
  /// @brief Score avant calcul final
  unsigned int saved_scores[LIBCARCASSONNE_MAX_PLAYERS];
  /// @brief Meeples retirés pour éviter les doublons (avec infos pour restauration)
  end_game_removed_meeples_vector_t removed_meeples;
};

/**
 * @brief Définis le hook pour placer une tile
 *
 */
LIBCARCASSONNE_HOOK_DEF(tile_place, 4, LIBCARCASSONNE_ACTION_PLACE_TILE)

/**
 * @brief Définis le hook pour placer un meeple
 *
 */
LIBCARCASSONNE_HOOK_DEF(meeple_place, 5, LIBCARCASSONNE_ACTION_PLACE_MEEPLE)

/**
 * @brief Hook de jeu qui restitue les meeples dans des groupes terminés (ex.
 * abbaye, route ou ville)
 *
 */
LIBCARCASSONNE_HOOK_DEF(give_back_meeples, 6, LIBCARCASSONNE_ACTION_NONE)

/**
 * @brief Change le player courrant au prochain
 *
 */
LIBCARCASSONNE_HOOK_DEF(next_player, 7, LIBCARCASSONNE_ACTION_NONE)

/**
 * @brief Vérifie si la partie est terminée
 *
 */
LIBCARCASSONNE_HOOK_DEF(end_game, 10, LIBCARCASSONNE_ACTION_NONE)

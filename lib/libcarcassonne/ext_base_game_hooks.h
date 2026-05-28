#pragma once

#include <libcarcassonne/extension.h>
#include <libcarcassonne/libcarcassonne.h>

/// @brief Informations pour restaurer la pose d'un meeple
struct meeple_place_hook_state {
  /// @brief Composante en x
  int x;
  /// @brief Composante en y
  int y;
  /// @brief Groupe sur lequel poser le meeple
  tile_part_group_t group;
  /// @brief Type du meeple à poser
  meeple_type_t meeple_type;
};

/// @brief Informations pour restaurer la pose d'une tuile
struct tile_place_hook_state {
  /// @brief Composante en x
  int x;
  /// @brief Composante en y
  int y;
};

/// @brief Informations pour restaurer le rendu de meeple
struct give_back_meeple_hook_state {
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

/// @brief Informations pour annuler la fin d'une partie
struct end_game_hook_state {
  /// @brief Score avant calcul final
  unsigned int saved_scores[LIBCARCASSONNE_MAX_PLAYERS];
  /// @brief Meeples retirés pour éviter les doublons (avec infos pour
  /// restauration)
  end_game_removed_meeples_vector_t removed_meeples;
};

/// @brief Informations pour annuler la pioche d'une tuile
struct pick_tile_hook_state {
  discarded_tile_list_t discarded_tiles;
  const tile_t *tile;
};

/// @brief État sauvegardé pour un groupe complet avec meeples
/// (give_back_meeples)
struct give_back_scored_group {
  unsigned int                      points;
  bool                              player_won[LIBCARCASSONNE_MAX_PLAYERS];
  end_game_removed_meeples_vector_t meeples;
};

/// @brief État sauvegardé pour la défausse d'une tuile impossible à jouer
struct discarded_tile {
  /// @brief L'emplacement mémoire de la tuile défaussée
  const tile_t **tile;
  /// @brief La position de la tuile défaussée au moment de la remise dans le deck
  unsigned int index;
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

/**
 * @brief Pioche une tuile et vérifie qu'elle est plaçable, en pioche une autre sinon
 *
 */
LIBCARCASSONNE_HOOK_HEADER(pick_tile)

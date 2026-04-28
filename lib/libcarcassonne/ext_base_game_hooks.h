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
LIBCARCASSONNE_HOOK_DEF(rendre_meeples, 6, LIBCARCASSONNE_ACTION_NONE)

/**
 * @brief Change le player courrant au prochain
 *
 */
LIBCARCASSONNE_HOOK_DEF(prochain_joueur, 7, LIBCARCASSONNE_ACTION_NONE)

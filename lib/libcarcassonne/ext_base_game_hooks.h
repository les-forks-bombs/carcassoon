#pragma once

#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/tile.h>

struct meeple_place_hook_state {
  int               x, y;
  tile_part_group_t group;
  meeple_type_t     meeple_type;
};

struct tile_place_hook_state {
  int x, y;
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
 * @brief Hook de jeu qui restitue les meeples dans des groupes terminés (ex. abbaye, route ou ville)
 * 
 */
LIBCARCASSONNE_HOOK_DEF(rendre_meeples, 6, LIBCARCASSONNE_ACTION_NONE)

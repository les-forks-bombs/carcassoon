#pragma once

#include <libcarcassonne/tile.h>

typedef enum meeple_type { BASIC, LARGE, ABBOT } meeple_type_t;

struct meeple {
  player_t*            player;
  placed_tile_group_t* group_node;
  meeple_type_t        meeple_type;
};

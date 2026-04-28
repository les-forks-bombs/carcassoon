#pragma once

#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>

typedef enum meeple_type { BASIC = 0, LARGE = 1, ABBOT = 2 } meeple_type_t;

struct meeple {
  player_t*            player;
  placed_tile_group_t* group_node;
  meeple_type_t        meeple_type;
};

struct meeple_count {
  meeple_type_t meeple_type;
  unsigned int  count;
};
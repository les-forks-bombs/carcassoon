#pragma once

#include <libcarcassonne/tile.h>

typedef struct meeple {
  unsigned int              player;
  struct placed_tile_group* group_node;
} meeple_t;

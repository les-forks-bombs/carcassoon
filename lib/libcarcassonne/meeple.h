#pragma once

#include <libcarcassonne/tile.h>

typedef enum meeple_type { BASIC, LARGE, ABBOT } meeple_type_t;

typedef struct meeple {
  unsigned int              player;
  struct placed_tile_group* group_node;
  meeple_type_t             meeple_type;
} meeple_t;

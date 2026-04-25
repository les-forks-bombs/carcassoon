#pragma once

#include <libcarcassonne/tile.h>

struct player;

typedef enum meeple_type { BASIC, LARGE, ABBOT } meeple_type_t;

typedef struct meeple {
  struct player*            player;
  struct placed_tile_group* group_node;
  meeple_type_t             meeple_type;
} meeple_t;

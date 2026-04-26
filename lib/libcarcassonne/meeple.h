#pragma once

#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>

typedef Vector(struct meeple*) meeple_list_t;
typedef Vector(struct meeple_count*) meeple_count_list_t;

typedef enum meeple_type { BASIC = 0, LARGE = 1, ABBOT = 2 } meeple_type_t;

typedef struct meeple {
  struct player*            player;
  struct placed_tile_group* group_node;
  meeple_type_t             meeple_type;
} meeple_t;

typedef struct meeple_count {
  meeple_type_t meeple_type;
  unsigned int  count;
} meeple_count_t;
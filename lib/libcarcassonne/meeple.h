#ifndef H_LIBCARCASSONNE_MEEPLE
#define H_LIBCARCASSONNE_MEEPLE

#include <libcarcassonne/tile.h>

typedef struct meeple {
  unsigned int player;
  struct placed_tile_group* tile_group;
} meeple_t;

#endif
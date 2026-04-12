#include "tile.h"

#include <memory.h>
#include <stdlib.h>

tile_part_type_t tile_get_family_face(tile_t*            tile,
                                      tile_orientation_t orientation,
                                      tile_orientation_t connexion_face) {
  if (tile == NULL) return 0;

  int        index     = (connexion_face + orientation) % 4;
  static int values[4] = {1, 5, 7, 3};

  return tile->parts[values[index]];
}

tile_orientation_t tile_orientation_invert(tile_orientation_t orientation) {
  return (orientation + 2) % 4;
}

placed_tile_group_t* tile_orientation_group(placed_tile_t*     tile,
                                            tile_orientation_t connexion_face) {
  if (tile == NULL) return 0;

  int        index     = (connexion_face + tile->orientation) % 4;
  static int values[4] = {1, 5, 7, 3};

  unsigned int groupe = tile->parent->parts_groups[values[index]];

  return tile->groups[groupe];
}

return_code_t placed_tile_create(placed_tile_t* placed_tile, tile_t* parent,
                                 tile_orientation_t orientation) {
  if (placed_tile == NULL || parent == NULL) return ERROR;

  placed_tile->parent      = parent;
  placed_tile->orientation = orientation;
  for (int i = 0; i < 9; i++) {
    placed_tile_group_t** group =
        &placed_tile->groups[placed_tile->parent->parts_groups[i]];
    if (*group == NULL) {
      *group         = calloc(1, sizeof(placed_tile_group_t));
      (*group)->tile = placed_tile;
    }
  }

  return SUCCESS;
}

void placed_tile_destroy(placed_tile_t* placed_tile) {
  bool freed[9];
  memset(&freed, 0, sizeof(freed));

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!freed[group]) free(placed_tile->groups[group]);
    freed[group] = true;
  }
}

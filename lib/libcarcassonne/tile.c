#include "tile.h"

#include <memory.h>
#include <stdlib.h>

tile_part_type_t tile_get_family_face(tile_t*            tile,
                                      tile_orientation_t orientation,
                                      tile_orientation_t connexion_face) {
  if (tile == NULL) return 0;

  int              index     = (connexion_face + orientation) % 4;
  static const int values[4] = {1, 5, 7, 3};

  return tile->parts[values[index]];
}

tile_orientation_t tile_orientation_invert(tile_orientation_t orientation) {
  return (orientation + 2) % 4;
}

return_code_t tile_get_face(placed_face_groups_t* ret, placed_tile_t* tile,
                            tile_orientation_t connexion_face) {
  if (tile == NULL) return ERROR;

  int index = (connexion_face + tile->orientation) % 4;

  static const int values[4][3] = {{0, 1, 2}, {2, 5, 8}, {8, 7, 6}, {6, 3, 0}};

  tile_part_group_t groups[3] = {tile->parent->parts_groups[values[index][0]],
                                 tile->parent->parts_groups[values[index][1]],
                                 tile->parent->parts_groups[values[index][2]]};

  ret->face[0] = tile->groups[groups[0]];
  ret->face[1] = tile->groups[groups[1]];
  ret->face[2] = tile->groups[groups[2]];

  return SUCCESS;
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

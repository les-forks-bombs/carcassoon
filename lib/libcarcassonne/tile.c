#include <libcarcassonne/libcarcassonne.h>

tile_part_type_t tile_get_family_face(const tile_t*      tile,
                                      tile_orientation_t orientation,
                                      tile_orientation_t connexion_face) {
  if (tile == NULL) {
    return 0;
  }

  int              index     = (connexion_face + orientation) % 4;
  static const int values[4] = {1, 5, 7, 3};

  return tile->parts[values[index]];
}

tile_orientation_t tile_orientation_invert(tile_orientation_t orientation) {
  return (orientation + 2) % 4;
}

#include <libcarcassonne/tile.h>
#include <libcarcassonne/tests/tests.h>

/// @brief Test que tile_orientation_invert(NORTH) retourne SOUTH
void tile_orientation_invert_north(void** state) {
  (void)state;
  assert_int_equal(tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH);
}

/// @brief Test que tile_orientation_invert(EAST) retourne WEST
void tile_orientation_invert_east(void** state) {
  (void)state;
  assert_int_equal(tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_EAST),
                   LIBCARCASSONNE_TILE_ORIENTATION_WEST);
}

/// @brief Test que tile_orientation_invert(SOUTH) retourne NORTH
void tile_orientation_invert_south(void** state) {
  (void)state;
  assert_int_equal(tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
}

/// @brief Test que tile_orientation_invert(WEST) retourne EAST
void tile_orientation_invert_west(void** state) {
  (void)state;
  assert_int_equal(tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_WEST),
                   LIBCARCASSONNE_TILE_ORIENTATION_EAST);
}

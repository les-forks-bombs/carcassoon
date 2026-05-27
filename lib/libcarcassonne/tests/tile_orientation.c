#include <libcarcassonne/tests/tests.h>
#include <libcarcassonne/tile.h>

void tile_orientation_invert_north(void** state) {
  (void)state;
  assert_int_equal(
      tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      LIBCARCASSONNE_TILE_ORIENTATION_SOUTH);
}

void tile_orientation_invert_east(void** state) {
  (void)state;
  assert_int_equal(
      tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      LIBCARCASSONNE_TILE_ORIENTATION_WEST);
}

void tile_orientation_invert_south(void** state) {
  (void)state;
  assert_int_equal(
      tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
      LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
}

void tile_orientation_invert_west(void** state) {
  (void)state;
  assert_int_equal(
      tile_orientation_invert(LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      LIBCARCASSONNE_TILE_ORIENTATION_EAST);
}

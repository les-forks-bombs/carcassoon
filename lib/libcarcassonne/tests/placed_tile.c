#include "libcarcassonne/placed_tile.h"
#include <libcarcassonne/deck.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>
#include "libcarcassonne/consts.h"
#include "libcarcassonne/ext_base_game.h"
#include "libcarcassonne/forward.h"
#include "libcarcassonne/tile.h"

/* Vérifie l'instanciation d'un deck */
void placed_tile_open_slots_works(void** state) {
  (void)state;

  placed_tile_t tile = {0};
  return_code_t code = placed_tile_create(&tile, &LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[12], LIBCARCASSONNE_TILE_ORIENTATION_NORTH, 0, 1);

  assert_int_equal(code, SUCCESS);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_A]->open_slots, 1);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_B]->open_slots, 2);
  assert_int_equal(tile.groups[LIBCARCASSONNE_TILE_PART_C]->open_slots, 1);

  placed_tile_destroy(&tile);
}
#include <libcarcassonne/engine.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/tests/tests.h>

#include "libcarcassonne/action.h"
#include "libcarcassonne/consts.h"
#include "libcarcassonne/deck.h"
#include "libcarcassonne/ext_base_game.h"
#include "libcarcassonne/game.h"
#include "libcarcassonne/meeple.h"
#include "libcarcassonne/tile.h"

void engine_builds(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);
  destroy_engine(&engine);
}

void engine_short_play_test(void** state) {
  (void)state;
  engine_t engine = {0};
  assert_int_equal(create_engine(&engine, options), SUCCESS);

  const tile_t* tile;

  tile = deck_find_tile(&engine.game.deck, "CFCF");
  assert_ptr_not_equal(tile, NULL);
  action_t action = {
      .type  = LIBCARCASSONNE_ACTION_PLACE_TILE,
      .order = {.place_tile = {
                    .tile        = tile,
                    .x           = -1,
                    .y           = 0,
                    .orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH}}};

  // assert_int_equal(engine.current_hook, 0);
  // assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
  // assert_int_equal(engine.current_hook, 0);

  assert_int_equal(start_game(&engine), SUCCESS);

  assert_int_equal(engine.current_hook, 0);
  assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);
  assert_int_equal(engine.current_hook, 1);

  action.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;

  action.order.place_meeple.meeple_type = BASIC;
  action.order.place_meeple.part_group  = B;
  action.order.place_meeple.tile        = *game_tile_at(&engine.game, -1, 0);
  action.order.place_meeple.x           = -1;
  action.order.place_meeple.y           = 0;

  assert_int_equal(dispatch_action(&engine, action), SUCCESS);
  assert_int_equal(engine.current_hook, 0);

  destroy_engine(&engine);
}

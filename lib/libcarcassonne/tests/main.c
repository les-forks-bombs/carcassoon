#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(deck_builds),
      cmocka_unit_test(deck_seed_consistent),
      cmocka_unit_test(game_builds),
      cmocka_unit_test(game_do_not_build_because_game_is_null),
      cmocka_unit_test(game_do_not_build_because_nb_players_too_low),
      cmocka_unit_test(game_do_not_build_because_nb_players_too_high),
      cmocka_unit_test(game_do_not_build_because_nb_ia_too_high),
      cmocka_unit_test(game_tile_at_works),
      cmocka_unit_test(game_tile_at_out_of_bounds),
      cmocka_unit_test(game_place_tile_works),
      cmocka_unit_test(game_place_tile_do_not_work_because_game_is_null),
      cmocka_unit_test(game_place_tile_do_not_work_because_tile_is_null),
      cmocka_unit_test(
          game_place_tile_do_not_work_because_tile_cannot_be_replaced),
      cmocka_unit_test(
          game_place_tile_do_not_work_because_position_is_out_of_bounds),
      cmocka_unit_test(game_place_tile_do_not_work_because_position_is_taken),
      cmocka_unit_test(game_place_multiple_tile_works),
      cmocka_unit_test(
          game_place_tile_do_not_work_because_tiles_are_incompatible),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

#ifndef H_TESTS
#define H_TESTS

#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/extension.h>
#include <libutils/cmocka.h>

static extension_list_t base = {
    .size       = 1,
    .extensions = &LIBCARCASSONNE_EXT_BASE_GAME,
};

void deck_builds(void** state);
void deck_seed_consistent(void** state);
void tile_get_family_face_works(void** state);
void game_builds(void** state);
void game_do_not_build_because_game_is_null(void** state);
void game_do_not_build_because_nb_players_too_low(void** state);
void game_do_not_build_because_nb_players_too_high(void** state);
void game_do_not_build_because_nb_ia_too_high(void** state);
void game_tile_at_works(void** state);
void game_tile_at_out_of_bounds(void** state);
void game_place_tile_works(void** state);
void game_place_tile_do_not_work_because_game_is_null(void** state);
void game_place_tile_do_not_work_because_tile_is_null(void** state);
void game_place_tile_do_not_work_because_tile_cannot_be_replaced(void** state);
void game_place_tile_do_not_work_because_position_is_out_of_bounds(
    void** state);
void game_place_tile_do_not_work_because_position_is_taken(void** state);
void game_place_multiple_tile_works(void** state);
void game_place_tile_do_not_work_because_tiles_are_incompatible(void** state);

void test_tile_group_builds(void** state);
void test_tile_group_link(void** state);
void test_tile_group_dynamic_connectivity(void** state);
void test_tile_group_cut(void** state);
void test_tile_group_merge_two_trees(void** state);

static const struct CMUnitTest tests[] = {
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
    cmocka_unit_test(tile_get_family_face_works),

    cmocka_unit_test(test_tile_group_builds),
    cmocka_unit_test(test_tile_group_link),
    cmocka_unit_test(test_tile_group_dynamic_connectivity),
    cmocka_unit_test(test_tile_group_cut),
    cmocka_unit_test(test_tile_group_merge_two_trees),
};

#endif

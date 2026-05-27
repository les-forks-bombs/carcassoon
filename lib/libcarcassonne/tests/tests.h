#ifndef H_TESTS
#define H_TESTS

#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/options.h>
#include <libutils/cmocka.h>

static const extension_t* base_ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};

static const extension_vector_t base = {.meta = {
                                            .size = 1,
                                            .caps = 1,
                                            .data = &base_ptr_table,
                                        }};

static options_t options = {
    .mode       = CARCASSONNE_MODE_CLI,
    .players    = 3,
    .seed       = 500,
    .ai         = 0,
    .max_turns  = 0,
    .extensions = base,
};

void deck_builds(void** state);
void deck_seed_consistent(void** state);
/* Deck extras tests */
void deck_pick_removes_tile(void** state);
void deck_pick_on_empty_returns_null(void** state);
void deck_defausser_adds_tile_back(void** state);
void deck_defausser_random_position(void** state);
void deck_find_tile_removes_match(void** state);
void deck_find_tile_returns_null_if_not_found(void** state);
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

void game_get_available_space_works(void** state);

/* Placed tile extras tests */
void placed_tile_destroy_handles_null(void** state);
void placed_tile_create_with_null(void** state);
void placed_tile_group_connected_with_linked_frrr_tiles(void** state);
void placed_tile_group_eval_points_single_road(void** state);

void test_tile_group_builds(void** state);
void test_tile_group_link(void** state);
void test_tile_group_dynamic_connectivity(void** state);
void test_tile_group_cut(void** state);
void test_tile_group_merge_two_trees(void** state);

void engine_builds(void** state);
void engine_long_play_test(void** state);
void engine_trigger_end_game(void** state);
void engine_trigger_give_back_meeple(void** state);

/* Tests pour prng_mersenne_twister */
void prng_create_with_seed_initializes_state(void** state);
void prng_random_consistent_sequence(void** state);
void prng_random_different_seeds_different_sequence(void** state);
void prng_random_values_in_valid_range(void** state);

/* Tests pour tile_orientation */
void tile_orientation_invert_north(void** state);
void tile_orientation_invert_east(void** state);
void tile_orientation_invert_south(void** state);
void tile_orientation_invert_west(void** state);

void placed_tile_open_slots_works(void** state);

/* Tests pour player */
void player_create_initializes_fields(void** state);
void player_free_works(void** state);
void player_free_handles_null(void** state);
void player_has_meeple_returns_true(void** state);
void player_has_meeple_returns_false(void** state);

/* Tests pour placed_tile - propagation des groupes et complétude */
void placed_tile_road_single_not_complete(void** state);
void placed_tile_town_cccc_not_complete(void** state);
void placed_tile_field_fcfc_not_complete(void** state);
void placed_tile_road_link_propagates_completeness(void** state);
void placed_tile_road_l_shape_not_complete(void** state);

/* Tests pour game_queries */
void game_is_place_open_true_with_empty_neighbors(void** state);
void game_is_place_open_false_when_all_neighbors_occupied(void** state);
void game_is_space_available_true_for_empty(void** state);
void game_is_space_available_false_when_occupied(void** state);
void game_is_tile_placeable_null_game_returns_false(void** state);
void game_is_tile_placeable_null_tile_returns_false(void** state);
void game_get_current_player_returns_first_player(void** state);
void game_get_current_player_advances_after_end_turn(void** state);

/* Tests pour game_meeple */
void game_place_meeple_success(void** state);
void game_place_meeple_on_occupied_group_fails(void** state);
void game_place_meeple_null_game_fails(void** state);
void game_place_meeple_none_is_noop(void** state);
void game_remove_meeple_success(void** state);
void game_remove_meeple_empty_group_succeeds(void** state);
void game_remove_meeple_null_game_fails(void** state);

/* Tests pour game_turn */
void game_end_player_turn_advances_player(void** state);
void game_end_player_turn_at_last_returns_no_more_player(void** state);
void game_end_round_resets_player_and_increments_turn(void** state);
void game_end_round_before_last_player_fails(void** state);
void is_game_finished_false_initially(void** state);
void is_game_finished_false_with_no_turns_limit(void** state);
void is_game_finished_true_when_turn_limit_reached(void** state);
void game_end_player_turn_null_game_fails(void** state);
void game_end_round_null_game_fails(void** state);

/* Tests pour game_remove_tile */
void game_remove_tile_success(void** state);
void game_remove_tile_empty_position_fails(void** state);
void game_remove_tile_out_of_bounds_fails(void** state);
void game_remove_tile_with_meeple_fails(void** state);

/* Tests pour scoring */
void scoring_road_two_tiles_two_points(void** state);
void scoring_town_incomplete_one_point(void** state);
void scoring_town_with_blason_incomplete(void** state);
void scoring_town_complete_flag_doubles_value(void** state);
void scoring_field_zero_points(void** state);
void scoring_village_zero_points(void** state);

/* Tests pour engine_actions */
void engine_wanted_action_is_place_tile_at_start(void** state);
void engine_get_actions_not_empty_at_start(void** state);
void engine_wanted_action_is_place_meeple_after_tile(void** state);
void engine_get_actions_meeple_includes_none_option(void** state);
void engine_get_actions_only_none_when_no_meeples(void** state);

/* Tests pour engine_revert */
void engine_revert_after_tile_removes_tile(void** state);
void engine_revert_full_turn_clears_board(void** state);
void engine_revert_partial_keeps_tile(void** state);
void engine_revert_restores_player_index(void** state);
void engine_revert_restores_meeple_count(void** state);
void engine_revert_dispatch_vector_size(void** state);
void engine_revert_current_hook_zero_on_empty(void** state);
void engine_revert_two_turns_to_epoch_five(void** state);
void engine_revert_end_game_state_restored(void** state);
void engine_revert_then_dispatch_works(void** state);
void engine_revert_long_play_with_revert(void** state);
void engine_revert_interleaved_revert_and_play(void** state);

static const struct CMUnitTest tests[] = {
    cmocka_unit_test(deck_builds),
    cmocka_unit_test(deck_seed_consistent),
    /* Deck extras */
    cmocka_unit_test(deck_pick_removes_tile),
    cmocka_unit_test(deck_pick_on_empty_returns_null),
    cmocka_unit_test(deck_defausser_adds_tile_back),
    cmocka_unit_test(deck_defausser_random_position),
    cmocka_unit_test(deck_find_tile_removes_match),
    cmocka_unit_test(deck_find_tile_returns_null_if_not_found),
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

    cmocka_unit_test(game_get_available_space_works),

    /* Placed tile extras */
    cmocka_unit_test(placed_tile_destroy_handles_null),
    cmocka_unit_test(placed_tile_create_with_null),
    cmocka_unit_test(placed_tile_group_connected_with_linked_frrr_tiles),
    cmocka_unit_test(placed_tile_group_eval_points_single_road),

    cmocka_unit_test(tile_get_family_face_works),

    cmocka_unit_test(test_tile_group_builds),
    cmocka_unit_test(test_tile_group_link),
    cmocka_unit_test(test_tile_group_dynamic_connectivity),
    cmocka_unit_test(test_tile_group_cut),
    cmocka_unit_test(test_tile_group_merge_two_trees),

    cmocka_unit_test(engine_builds),
    cmocka_unit_test(engine_long_play_test),
    cmocka_unit_test(engine_trigger_end_game),

    cmocka_unit_test(placed_tile_open_slots_works),
    cmocka_unit_test(placed_tile_road_single_not_complete),
    cmocka_unit_test(placed_tile_town_cccc_not_complete),
    cmocka_unit_test(placed_tile_field_fcfc_not_complete),
    cmocka_unit_test(placed_tile_road_link_propagates_completeness),
    cmocka_unit_test(placed_tile_road_l_shape_not_complete),
    cmocka_unit_test(engine_trigger_give_back_meeple),

    /* PRNG tests */
    cmocka_unit_test(prng_create_with_seed_initializes_state),
    cmocka_unit_test(prng_random_consistent_sequence),
    cmocka_unit_test(prng_random_different_seeds_different_sequence),
    cmocka_unit_test(prng_random_values_in_valid_range),

    /* Tile orientation tests */
    cmocka_unit_test(tile_orientation_invert_north),
    cmocka_unit_test(tile_orientation_invert_east),
    cmocka_unit_test(tile_orientation_invert_south),
    cmocka_unit_test(tile_orientation_invert_west),

    /* Player tests */
    cmocka_unit_test(player_create_initializes_fields),
    cmocka_unit_test(player_free_works),
    cmocka_unit_test(player_free_handles_null),
    cmocka_unit_test(player_has_meeple_returns_true),
    cmocka_unit_test(player_has_meeple_returns_false),

    /* game_queries */
    cmocka_unit_test(game_is_place_open_true_with_empty_neighbors),
    cmocka_unit_test(game_is_place_open_false_when_all_neighbors_occupied),
    cmocka_unit_test(game_is_space_available_true_for_empty),
    cmocka_unit_test(game_is_space_available_false_when_occupied),
    cmocka_unit_test(game_is_tile_placeable_null_game_returns_false),
    cmocka_unit_test(game_is_tile_placeable_null_tile_returns_false),
    cmocka_unit_test(game_get_current_player_returns_first_player),
    cmocka_unit_test(game_get_current_player_advances_after_end_turn),

    /* game_meeple */
    cmocka_unit_test(game_place_meeple_success),
    cmocka_unit_test(game_place_meeple_on_occupied_group_fails),
    cmocka_unit_test(game_place_meeple_null_game_fails),
    cmocka_unit_test(game_place_meeple_none_is_noop),
    cmocka_unit_test(game_remove_meeple_success),
    cmocka_unit_test(game_remove_meeple_empty_group_succeeds),
    cmocka_unit_test(game_remove_meeple_null_game_fails),

    /* game_turn */
    cmocka_unit_test(game_end_player_turn_advances_player),
    cmocka_unit_test(game_end_player_turn_at_last_returns_no_more_player),
    cmocka_unit_test(game_end_round_resets_player_and_increments_turn),
    cmocka_unit_test(game_end_round_before_last_player_fails),
    cmocka_unit_test(is_game_finished_false_initially),
    cmocka_unit_test(is_game_finished_false_with_no_turns_limit),
    cmocka_unit_test(is_game_finished_true_when_turn_limit_reached),
    cmocka_unit_test(game_end_player_turn_null_game_fails),
    cmocka_unit_test(game_end_round_null_game_fails),

    /* game_remove_tile */
    cmocka_unit_test(game_remove_tile_success),
    cmocka_unit_test(game_remove_tile_empty_position_fails),
    cmocka_unit_test(game_remove_tile_out_of_bounds_fails),
    cmocka_unit_test(game_remove_tile_with_meeple_fails),

    /* scoring */
    cmocka_unit_test(scoring_road_two_tiles_two_points),
    cmocka_unit_test(scoring_town_incomplete_one_point),
    cmocka_unit_test(scoring_town_with_blason_incomplete),
    cmocka_unit_test(scoring_town_complete_flag_doubles_value),
    cmocka_unit_test(scoring_field_zero_points),
    cmocka_unit_test(scoring_village_zero_points),

    /* engine_actions */
    cmocka_unit_test(engine_wanted_action_is_place_tile_at_start),
    cmocka_unit_test(engine_get_actions_not_empty_at_start),
    cmocka_unit_test(engine_wanted_action_is_place_meeple_after_tile),
    cmocka_unit_test(engine_get_actions_meeple_includes_none_option),
    cmocka_unit_test(engine_get_actions_only_none_when_no_meeples),

    /* engine_revert */
    cmocka_unit_test(engine_revert_after_tile_removes_tile),
    cmocka_unit_test(engine_revert_full_turn_clears_board),
    cmocka_unit_test(engine_revert_partial_keeps_tile),
    cmocka_unit_test(engine_revert_restores_player_index),
    cmocka_unit_test(engine_revert_restores_meeple_count),
    cmocka_unit_test(engine_revert_dispatch_vector_size),
    cmocka_unit_test(engine_revert_current_hook_zero_on_empty),
    cmocka_unit_test(engine_revert_two_turns_to_epoch_five),
    cmocka_unit_test(engine_revert_end_game_state_restored),
    cmocka_unit_test(engine_revert_then_dispatch_works),
    cmocka_unit_test(engine_revert_long_play_with_revert),
    cmocka_unit_test(engine_revert_interleaved_revert_and_play),

};

#endif

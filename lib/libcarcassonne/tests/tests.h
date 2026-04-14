#ifndef H_TESTS
#define H_TESTS

/** DECK TEST */
void deck_builds(void** state);
void deck_seed_consistent(void** state);

/** GAME TEST */
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

#endif

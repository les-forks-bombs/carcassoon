#include <libcarcassonne/game.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>
#include <string.h>

#include <libcarcassonne/options.h>

/** create_game */

const static options_t options = {
    .mode      = CARCASSONNE_MODE_CLI,
    .players   = 3,
    .seed      = 500,
    .ai        = 0,
    .max_turns = 0,
    .extensions =
        {
            .extensions = &LIBCARCASSONNE_EXT_BASE_GAME,
            .size       = 1,
        },
};

/* Vérifie l'instanciation d'une game */
void game_builds(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);
  destroy_game(&game);
}

void game_do_not_build_because_game_is_null(void** state) {
  (void)state;
  assert_int_equal(create_game(NULL, &options), ERROR);
}

void game_do_not_build_because_nb_players_too_low(void** state) {
  (void)state;
  game_t game;

  options_t t = {
      .mode      = CARCASSONNE_MODE_CLI,
      .players   = 0,
      .seed      = 500,
      .ai        = 0,
      .max_turns = 0,
  };
  assert_int_equal(create_game(&game, &t), ERROR);
}

void game_do_not_build_because_nb_players_too_high(void** state) {
  (void)state;
  options_t t = {
      .mode      = CARCASSONNE_MODE_CLI,
      .players   = 6,
      .seed      = 500,
      .ai        = 0,
      .max_turns = 0,
  };
  game_t game;
  assert_int_equal(create_game(&game, &t), ERROR);
}

void game_do_not_build_because_nb_ia_too_high(void** state) {
  (void)state;
  options_t t = {
      .mode      = CARCASSONNE_MODE_CLI,
      .players   = 3,
      .seed      = 500,
      .ai        = 4,
      .max_turns = 0,
  };
  game_t game;
  assert_int_equal(create_game(&game, &t), ERROR);
}

/** game_tile_at */

void game_tile_at_works(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  placed_tile_t** tile = game_tile_at(&game, -71, -71);
  assert_ptr_equal(tile, &game.map[0]);

  destroy_game(&game);
}

void game_tile_at_out_of_bounds(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  placed_tile_t** placed_tile = game_tile_at(&game, -200, 200);
  assert_ptr_equal(placed_tile, NULL);

  destroy_game(&game);
}

/** game_place_tile */

void game_place_tile_works(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_game_is_null(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(game_place_tile(NULL, tile, -200, 200,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   ERROR);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_tile_is_null(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  assert_int_equal(
      game_place_tile(&game, NULL, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      ERROR);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_tile_cannot_be_replaced(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      NOT_FREE);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_position_is_out_of_bounds(
    void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(game_place_tile(&game, tile, -200, 200,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   OUT_OF_BOUNDS);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_position_is_taken(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      NOT_FREE);

  destroy_game(&game);
}

const tile_t* find_tile(game_t* game, char* family) {
  deck_element_t* curr = game->deck.list.head;
  while (strcmp(curr->tile->family, family) != 0) {
    curr = curr->next;
  }

  return curr->tile;
}

void game_place_multiple_tile_works(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  /** AJOUT BAS */
  tile = find_tile(&game, "CFCF");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT HAUT */
  tile = find_tile(&game, "CFCF");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  /** AJOUT DROITE */
  tile = find_tile(&game, "CCRR");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  /** AJOUT GAUCHE */
  tile = find_tile(&game, "FFFR");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, 0, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT HAUT GAUCHE */
  tile = find_tile(&game, "FFRR");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_WEST),
                   SUCCESS);

  /** AJOUT HAUT DROIT */
  tile = find_tile(&game, "CFFF");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT BAS GAUCHE */
  tile = find_tile(&game, "CFFF");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT BAS DROIT */
  tile = find_tile(&game, "CRRR");
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      SUCCESS);

  destroy_game(&game);
}

void game_place_tile_do_not_work_because_tiles_are_incompatible(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);

  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  deck_element_t* curr = game.deck.list.head;
  while (strcmp(curr->tile->family, "FFFF") != 0) {
    curr = curr->next;
  }
  const tile_t* tile2 = curr->tile;
  assert_ptr_not_equal(tile2, NULL);

  assert_int_equal(game_place_tile(&game, tile2, -1, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   INVALID_PLACEMENT);

  destroy_game(&game);
}

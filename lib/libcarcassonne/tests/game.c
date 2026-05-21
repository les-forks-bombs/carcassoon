#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "libcarcassonne/forward.h"
#include "libutils/vector.h"

/** create_game */

/* Vérifie l'instanciation d'une game */
void game_builds(void** state) {
  (void)state;
  game_t game = {0};
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
  tile = deck_find_tile(&game.deck, "CFCF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT HAUT */
  tile = deck_find_tile(&game.deck, "CFCF", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  /** AJOUT DROITE */
  tile = deck_find_tile(&game.deck, "CCRR", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  /** AJOUT GAUCHE */
  tile = deck_find_tile(&game.deck, "FFFR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, 0, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT HAUT GAUCHE */
  tile = deck_find_tile(&game.deck, "FFRR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_WEST),
                   SUCCESS);

  /** AJOUT HAUT DROIT */
  tile = deck_find_tile(&game.deck, "CFFF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT BAS GAUCHE */
  tile = deck_find_tile(&game.deck, "CFFF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT BAS DROIT */
  tile = deck_find_tile(&game.deck, "CRRR", false);
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

  list_node_t* curr = list_head(&game.deck.list);
  while (strcmp((*list_value(&game.deck.list, curr))->family, "FFFF") != 0) {
    curr = curr->next;
  }
  const tile_t* tile2 = *list_value(&game.deck.list, curr);
  assert_ptr_not_equal(tile2, NULL);

  assert_int_equal(game_place_tile(&game, tile2, -1, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   INVALID_PLACEMENT);

  destroy_game(&game);
}

void game_get_available_space_works(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_pick(&game.deck);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  /** AJOUT BAS */
  tile = deck_find_tile(&game.deck, "CFCF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT HAUT */
  tile = deck_find_tile(&game.deck, "CFCF", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 0,
                                   LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
                   SUCCESS);

  /** AJOUT DROITE */
  tile = deck_find_tile(&game.deck, "CCRR", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  /** AJOUT GAUCHE */
  tile = deck_find_tile(&game.deck, "FFFR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, 0, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT HAUT GAUCHE */
  tile = deck_find_tile(&game.deck, "FFRR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, -1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_WEST),
                   SUCCESS);

  /** AJOUT HAUT DROIT */
  tile = deck_find_tile(&game.deck, "CFFF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(game_place_tile(&game, tile, -1, 1,
                                   LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
                   SUCCESS);

  /** AJOUT BAS GAUCHE */
  tile = deck_find_tile(&game.deck, "CFFF", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, -1, LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      SUCCESS);

  /** AJOUT BAS DROIT */
  tile = deck_find_tile(&game.deck, "CRRR", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 1, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      SUCCESS);

  /** AJOUT DROIT DROIT */
  tile = deck_find_tile(&game.deck, "CCCC", true);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 2, LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      SUCCESS);

  vector2d_vector_t vec = game_get_available_space(&game);

  vector2d_t correct_vec[12] = {
      {.x = 2, .y = 0},   {.x = -2, .y = 0},  {.x = 0, .y = -2},
      {.x = -2, .y = -1}, {.x = -1, .y = -2}, {.x = -2, .y = 1},
      {.x = -1, .y = 2},  {.x = 2, .y = -1},  {.x = 1, .y = -2},
      {.x = 2, .y = 1},   {.x = 1, .y = 2},   {.x = 0, .y = 3}};

  for (unsigned int i = 0; i < vector_size(&vec); i++) {
    vector2d_t spot = *vector_nth(&vec, i);
    assert_int_equal(spot.x, correct_vec[i].x);
    assert_int_equal(spot.y, correct_vec[i].y);
  }

  vector_free(&vec);

  destroy_game(&game);
}

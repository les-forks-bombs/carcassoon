
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

#include "libutils/vector.h"

// FRRR : parts_groups = {A,A,A,B,G,C,E,D,F}
// Groupe B = ROAD (face OUEST, position 3)

void game_place_meeple_success(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  player_t* player = &game.players[0];
  int       before =
      ((meeple_count_t*)vector_nth(&player->meeples_count, BASIC))->count;

  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      SUCCESS);

  // Le compteur de meeples doit avoir diminué de 1
  int after =
      ((meeple_count_t*)vector_nth(&player->meeples_count, BASIC))->count;
  assert_int_equal(after, before - 1);

  // Le groupe doit contenir un meeple
  placed_tile_t** p = game_tile_at(&game, 0, 0);
  assert_non_null(*p);
  assert_non_null((*p)->groups[LIBCARCASSONNE_TILE_PART_B]->meeple);

  destroy_game(&game);
}

void game_place_meeple_on_occupied_group_fails(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  player_t* player = &game.players[0];
  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      SUCCESS);

  // Deuxième placement sur le même groupe → doit échouer
  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      ALREADY_ALLOCATED);

  destroy_game(&game);
}

void game_place_meeple_null_game_fails(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  player_t* player = &game.players[0];
  assert_int_equal(
      game_place_meeple(NULL, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      ERROR);

  destroy_game(&game);
}

void game_place_meeple_none_is_noop(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  player_t* player = &game.players[0];
  // Type NONE = pas de placement, mais SUCCESS attendu
  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, NONE, player),
      SUCCESS);

  // Aucun meeple ne doit avoir été placé
  placed_tile_t** p = game_tile_at(&game, 0, 0);
  assert_null((*p)->groups[LIBCARCASSONNE_TILE_PART_B]->meeple);

  destroy_game(&game);
}

void game_remove_meeple_success(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  player_t* player = &game.players[0];
  int       before =
      ((meeple_count_t*)vector_nth(&player->meeples_count, BASIC))->count;

  assert_int_equal(
      game_place_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B, BASIC, player),
      SUCCESS);
  assert_int_equal(game_remove_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B),
                   SUCCESS);

  // Le meeple doit être rendu
  int after =
      ((meeple_count_t*)vector_nth(&player->meeples_count, BASIC))->count;
  assert_int_equal(after, before);

  placed_tile_t** p = game_tile_at(&game, 0, 0);
  assert_null((*p)->groups[LIBCARCASSONNE_TILE_PART_B]->meeple);

  destroy_game(&game);
}

void game_remove_meeple_empty_group_succeeds(void** state) {
  (void)state;
  game_t game;
  assert_int_equal(create_game(&game, &options), SUCCESS);

  const tile_t* tile = deck_find_tile(&game.deck, "FRRR", false);
  assert_non_null(tile);
  assert_int_equal(
      game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      SUCCESS);

  // Aucun meeple dans le groupe B → remove doit retourner SUCCESS sans crasher
  assert_int_equal(game_remove_meeple(&game, 0, 0, LIBCARCASSONNE_TILE_PART_B),
                   SUCCESS);

  destroy_game(&game);
}

void game_remove_meeple_null_game_fails(void** state) {
  (void)state;
  assert_int_equal(game_remove_meeple(NULL, 0, 0, LIBCARCASSONNE_TILE_PART_B),
                   ERROR);
}

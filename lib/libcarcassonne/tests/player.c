#include <libcarcassonne/meeple.h>
#include <libcarcassonne/player.h>
#include <libcarcassonne/tests/tests.h>
#include <stdlib.h>

void player_create_initializes_fields(void** state) {
  (void)state;

  meeple_count_vector_t meeples_count;
  vector_alloc(&meeples_count, 1);

  meeple_count_t basic_meeple = {.meeple_type = BASIC, .count = 7};
  vector_append(&meeples_count, &basic_meeple);

  player_t player =
      create_player(LIBCARCASSONNE_PLAYER_HUMAN, &meeples_count, 0);

  assert_int_equal(player.id, 0);
  assert_int_equal(player.player_type, LIBCARCASSONNE_PLAYER_HUMAN);
  assert_int_equal(player.score, 0);
  assert_int_equal(vector_size(&player.meeples_count), 1);

  vector_free(&meeples_count);
  free_player(&player);
}

void player_free_works(void** state) {
  (void)state;

  meeple_count_vector_t meeples_count;
  vector_alloc(&meeples_count, 1);

  meeple_count_t basic_meeple = {.meeple_type = BASIC, .count = 7};
  vector_append(&meeples_count, &basic_meeple);

  player_t* player = malloc(sizeof(player_t));
  *player          = create_player(LIBCARCASSONNE_PLAYER_AI, &meeples_count, 1);

  free_player(player);
  free(player);
  vector_free(&meeples_count);
}

void player_free_handles_null(void** state) {
  (void)state;
  free_player(NULL);
}

void player_has_meeple_returns_true(void** state) {
  (void)state;

  meeple_count_vector_t meeples_count;
  vector_alloc(&meeples_count, 1);

  meeple_count_t basic_meeple = {.meeple_type = BASIC, .count = 3};
  vector_append(&meeples_count, &basic_meeple);

  player_t player =
      create_player(LIBCARCASSONNE_PLAYER_HUMAN, &meeples_count, 0);

  assert_true(player_has_meeple_to_place(&player));

  vector_free(&meeples_count);
  free_player(&player);
}

void player_has_meeple_returns_false(void** state) {
  (void)state;

  meeple_count_vector_t meeples_count;
  vector_alloc(&meeples_count, 2);

  meeple_count_t basic_meeple = {.meeple_type = BASIC, .count = 0};
  meeple_count_t large_meeple = {.meeple_type = LARGE, .count = 0};
  vector_append(&meeples_count, &basic_meeple);
  vector_append(&meeples_count, &large_meeple);

  player_t player =
      create_player(LIBCARCASSONNE_PLAYER_HUMAN, &meeples_count, 0);

  assert_false(player_has_meeple_to_place(&player));

  vector_free(&meeples_count);
  free_player(&player);
}

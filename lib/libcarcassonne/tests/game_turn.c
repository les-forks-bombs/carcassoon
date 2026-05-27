#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>
#include <libcarcassonne/tests/tests.h>

// Options locales pour que max_turns soit maîtrisé indépendamment du global
static const extension_t* local_base_ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};
static const extension_vector_t local_base = {
    .meta = {.size = 1, .caps = 1, .data = &local_base_ptr_table}};

static options_t make_options(unsigned int max_turns) {
  options_t o = {.mode       = CARCASSONNE_MODE_CLI,
                 .players    = 3,
                 .seed       = 500,
                 .ai         = 0,
                 .max_turns  = max_turns,
                 .extensions = local_base};
  return o;
}

void game_end_player_turn_advances_player(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  assert_int_equal(game.current_player, 0);
  assert_int_equal(game_end_player_turn(&game), SUCCESS);
  assert_int_equal(game.current_player, 1);

  destroy_game(&game);
}

void game_end_player_turn_at_last_returns_no_more_player(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  // Avancer jusqu'au dernier joueur (index 2 sur 3)
  assert_int_equal(game_end_player_turn(&game), SUCCESS);  // 0 → 1
  assert_int_equal(game_end_player_turn(&game), SUCCESS);  // 1 → 2
  // 2 est le dernier, un appel de plus doit renvoyer NO_MORE_PLAYER
  assert_int_equal(game_end_player_turn(&game), NO_MORE_PLAYER);

  destroy_game(&game);
}

void game_end_round_resets_player_and_increments_turn(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  unsigned int turn_before = game.turn;

  // Amener le jeu au dernier joueur
  assert_int_equal(game_end_player_turn(&game), SUCCESS);
  assert_int_equal(game_end_player_turn(&game), SUCCESS);
  assert_int_equal(game_end_player_turn(&game), NO_MORE_PLAYER);

  assert_int_equal(game_end_round(&game), SUCCESS);

  assert_int_equal(game.current_player, 0);
  assert_int_equal(game.turn, turn_before + 1);

  destroy_game(&game);
}

void game_end_round_before_last_player_fails(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  // current_player = 0, pas le dernier → PLAYER_NOT_CALLED
  assert_int_equal(game_end_round(&game), PLAYER_NOT_CALLED);

  destroy_game(&game);
}

void is_game_finished_false_initially(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  // Deck non vide + turns_limit = 0 → pas terminé
  assert_false(is_game_finished(&game));

  destroy_game(&game);
}

void is_game_finished_false_with_no_turns_limit(void** state) {
  (void)state;
  options_t o = make_options(0);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  // Même avec un tour élevé, turns_limit = 0 = pas de limite
  game.turn = 100;
  assert_false(is_game_finished(&game));

  destroy_game(&game);
}

void is_game_finished_true_when_turn_limit_reached(void** state) {
  (void)state;
  options_t o = make_options(5);
  game_t    game;
  assert_int_equal(create_game(&game, &o), SUCCESS);

  game.turn = 5;  // turn >= turns_limit → terminé
  assert_true(is_game_finished(&game));

  destroy_game(&game);
}

void game_end_player_turn_null_game_fails(void** state) {
  (void)state;
  assert_int_equal(game_end_player_turn(NULL), NULL_POINTER);
}

void game_end_round_null_game_fails(void** state) {
  (void)state;
  assert_int_equal(game_end_round(NULL), NULL_POINTER);
}

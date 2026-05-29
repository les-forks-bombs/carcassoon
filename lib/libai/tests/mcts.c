#include <libai/tests/tests.h>
#include <libcarcassonne/ext_base_game.h>
#include <libutils/vector.h>
#include <stdio.h>

static options_t ai_test_options(unsigned int players, unsigned int max_turns) {
  static const extension_t*       ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};
  static const extension_vector_t ext         = {
      .meta = {.size = 1, .caps = 1, .data = &ptr_table}};
  options_t o = {.mode       = CARCASSONNE_MODE_CLI,
                 .players    = players,
                 .seed       = 42,
                 .ai         = 0,
                 .max_turns  = max_turns,
                 .extensions = ext};
  return o;
}

/* ai_play_turn ne plante pas avec 1 itération. */
void ai_play_turn_does_not_crash(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 5);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  ai_play_turn(&engine, 1);

  destroy_engine(&engine);
}

/* Après ai_play_turn, le vecteur dispatchs contient exactement 7 entrées :
 * pick_tile (start_game), tile_place, meeple_place, give_back_meeples,
 * next_player, end_game, pick_tile (tour suivant). */
void ai_play_turn_completes_full_turn(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);
  ai_play_turn(&engine, 100);
  assert_int_equal(vector_size(&engine.dispatchs), 7);
  destroy_engine(&engine);
}

/* Après ai_play_turn, le joueur courant est passé de 0 à 1. */
void ai_play_turn_advances_current_player(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  assert_int_equal(engine.game.current_player, 0);
  ai_play_turn(&engine, 100);
  assert_int_equal(engine.game.current_player, 1);

  destroy_engine(&engine);
}

/* Après ai_play_turn, le moteur attend une action PLACE_TILE pour le joueur
 * suivant (current_hook == 1 : pick_tile a déjà été auto-dispatché). */
void ai_play_turn_engine_ready_for_next_tile(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  ai_play_turn(&engine, 100);

  assert_int_equal(engine_wanted_action(&engine),
                   LIBCARCASSONNE_ACTION_PLACE_TILE);
  assert_int_equal(engine.current_hook, 1);

  destroy_engine(&engine);
}

/* ai_play_turn fonctionne sans planter avec 100 itérations et
 * produit toujours un tour complet (7 dispatches). */
void ai_play_turn_many_iterations_does_not_crash(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  ai_play_turn(&engine, 100);

  assert_int_equal(vector_size(&engine.dispatchs), 7);

  destroy_engine(&engine);
}

/* Deux appels consécutifs à ai_play_turn (un par joueur) complètent un round :
 * 13 dispatches au total (1 pick_tile start_game + 6 par appel) et
 * engine.game.turn vaut 1. */
void ai_play_turn_two_calls_advance_round(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  ai_play_turn(&engine, 100);  // joueur 0
  ai_play_turn(&engine, 100);  // joueur 1 → fin du round

  assert_int_equal(vector_size(&engine.dispatchs), 13);
  assert_int_equal(engine.game.turn, 1);

  destroy_engine(&engine);
}

/* Partie complète (max_turns=0) : 2 joueurs s'affrontent jusqu'au vidage du
 * deck. Seed fixée à 42 pour la reproductibilité. */
void ai_play_turn_full_game_two_players(void** state) {
  (void)state;
  options_t o      = ai_test_options(2, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  int turns_played = 0;
  while (engine.game.state != GAME_STATE_FINISHED) {
    ai_play_turn(&engine, 100);
    turns_played++;
  }

  assert_true(is_game_finished(&engine.game));

  printf("[full game] %d tours joués\n", turns_played);
  for (unsigned int i = 0; i < 2; i++) {
    printf("[full game] joueur %u : %u points\n", i,
           engine.game.players[i].score);
  }

  destroy_engine(&engine);
}


#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/vector.h>
#include <stdio.h>

#include "libcarcassonne/game.h"

static options_t full_game_opts(void) {
  static const extension_t*       ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};
  static const extension_vector_t ext         = {
      .meta = {.size = 1, .caps = 1, .data = &ptr_table}};
  options_t o = {.mode       = CARCASSONNE_MODE_CLI,
                 .players    = 3,
                 .seed       = 42,
                 .ai         = 0,
                 .max_turns  = 0,
                 .extensions = ext};
  return o;
}

/*
 * Joue un tour complet : placement de tuile puis de meeple.
 * tile_strat / meeple_strat : valeur modulo le nombre d'actions disponibles,
 * ce qui permet de choisir des actions différentes selon le contexte.
 */
static void play_turn(engine_t* engine, unsigned int tile_strat,
                      unsigned int meeple_strat) {
  action_vector_t actions = engine_get_actions(engine);
  for (unsigned int i = 0; i < vector_size(&actions); i++) {
    action_t* a = vector_nth(&actions, i);
  }
  assert_true(vector_size(&actions) > 0);
  unsigned int idx = tile_strat % (unsigned int)vector_size(&actions);
  action_t     act = *(action_t*)vector_nth(&actions, idx);
  vector_free(&actions);
  assert_int_equal(dispatch_action(engine, act), NO_PROGRESS);

  actions = engine_get_actions(engine);
  assert_true(vector_size(&actions) > 0);
  idx = meeple_strat % (unsigned int)vector_size(&actions);
  act = *(action_t*)vector_nth(&actions, idx);
  assert_int_equal(dispatch_action(engine, act), SUCCESS);
  vector_free(&actions);
}

/*
 * Test d'intégration complet : une partie entière à 3 joueurs (seed=42)
 * avec 3 reverts intercalés utilisant chaque fois des séquences d'actions
 * différentes de celles qui viennent d'être annulées.
 *
 * Structure :
 *   Phase 1  : 5 tours   (strat 0,0)
 *   Revert 1 : 2 leurres → retour epoch_r1 → 2 tours (strat 1,2 puis 2,1)
 *   Phase 2  : 5 tours   (strat 0,0)
 *   Revert 2 : 1 leurre  → retour epoch_r2 → 1 tour  (strat 3,0)
 *   Phase 3  : 35 tours  (strat 0,0)
 *   Revert 3 : 1 leurre  → retour epoch_r3 → 1 tour  (strat 1,1)
 *   Boucle finale jusqu'à GAME_STATE_FINISHED
 */
void engine_full_game_with_reverts(void** state) {
  (void)state;
  options_t o      = full_game_opts();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  /* ── Phase 1 : 5 tours normaux ─────────────────────────────────────── */
  for (int i = 0; i < 5; i++) {
    play_turn(&engine, 0, 0);
  }
  unsigned int epoch_r1 = (unsigned int)vector_size(&engine.dispatchs);

  /* ── Revert 1 ───────────────────────────────────────────────────────── */
  play_turn(&engine, 0, 0); /* leurre A */
  play_turn(&engine, 0, 0); /* leurre B */

  assert_int_equal(engine_revert(&engine, epoch_r1), SUCCESS);
  assert_int_equal((unsigned int)vector_size(&engine.dispatchs), epoch_r1);

  /* Rejoue les 2 tours avec des choix différents */
  play_turn(&engine, 1, 2);

  play_turn(&engine, 2, 1);

  /* ── Phase 2 : 5 tours supplémentaires ─────────────────────────────── */
  for (int i = 0; i < 5; i++) {
    play_turn(&engine, 0, 0);
  }
  unsigned int epoch_r2 = (unsigned int)vector_size(&engine.dispatchs);

  /* ── Revert 2 ───────────────────────────────────────────────────────── */
  play_turn(&engine, 0, 3); /* leurre */
  assert_int_equal(engine_revert(&engine, epoch_r2), SUCCESS);
  assert_int_equal((unsigned int)vector_size(&engine.dispatchs), epoch_r2);
  play_turn(&engine, 3, 0); /* choix différent du leurre */

  /* ── Phase 3 : 35 tours (ou fin anticipée) ──────────────────────────── */
  while (engine.game.state != GAME_STATE_FINISHED) {
    play_turn(&engine, 0, 0);
  }
  unsigned int epoch_r3 = (unsigned int)vector_size(&engine.dispatchs);

  /* ── Revert 3 (si la partie n'est pas encore terminée) ──────────────── */
  if (engine.game.state != GAME_STATE_FINISHED) {
    play_turn(&engine, 2, 2); /* leurre */
    assert_int_equal(engine_revert(&engine, epoch_r3), SUCCESS);
    assert_int_equal((unsigned int)vector_size(&engine.dispatchs), epoch_r3);
    play_turn(&engine, 1, 1); /* choix différent du leurre */
  }

  /* ── Boucle finale ──────────────────────────────────────────────────── */
  while (!is_game_finished(&engine.game)) {
    play_turn(&engine, 0, 0);
  }

  /* ── Affichage des scores ───────────────────────────────────────────── */
  printf("\n=== Scores finaux (seed=42, 3 joueurs) ===\n");
  for (unsigned int i = 0; i < 3; i++) {
    printf("  Joueur %u : %u points\n", i, engine.game.players[i].score);
  }

  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  destroy_engine(&engine);
}

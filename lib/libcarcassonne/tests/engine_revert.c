#include <cmocka.h>
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/libcarcassonne.h>

#include "libutils/vector.h"

static options_t revert_opts(unsigned int players, unsigned int max_turns) {
  static const extension_t*       ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};
  static const extension_vector_t ext         = {
      .meta = {.size = 1, .caps = 1, .data = &ptr_table}};
  options_t o = {.mode       = CARCASSONNE_MODE_CLI,
                 .players    = players,
                 .seed       = 500,
                 .ai         = 0,
                 .max_turns  = max_turns,
                 .extensions = ext};
  return o;
}

static action_t tile_action(const tile_t* tile, int x, int y,
                            tile_orientation_t ori) {
  action_t a                     = {0};
  a.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
  a.order.place_tile.tile        = tile;
  a.order.place_tile.x           = x;
  a.order.place_tile.y           = y;
  a.order.place_tile.orientation = ori;
  return a;
}

static action_t none_meeple_action(void) {
  action_t a                       = {0};
  a.type                           = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
  a.order.place_meeple.meeple_type = NONE;
  return a;
}

/* Après dispatch d'une seule tuile, engine_revert(0) la retire du plateau. */
void engine_revert_after_tile_removes_tile(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  /* FRFR en (1,0) NORTH : face N=FIELD compatible avec face S=FIELD de CRFR */
  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);
  action_t a = tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
  assert_int_equal(dispatch_action(&engine, a), NO_PROGRESS);

  assert_non_null(*game_tile_at(&engine.game, 1, 0));
  assert_int_equal(vector_size(&engine.dispatchs), 1);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  assert_null(*game_tile_at(&engine.game, 1, 0));
  assert_int_equal(vector_size(&engine.dispatchs), 0);

  destroy_engine(&engine);
}

/* Après un tour complet (5 dispatches), engine_revert(0) vide entièrement le
 * plateau et réinitialise l'index joueur. */
void engine_revert_full_turn_clears_board(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);
  assert_int_equal(vector_size(&engine.dispatchs), 5);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  assert_null(*game_tile_at(&engine.game, 1, 0));
  assert_int_equal(vector_size(&engine.dispatchs), 0);
  assert_int_equal(engine.current_hook, 0);

  destroy_engine(&engine);
}

/* engine_revert(1) après un tour complet conserve la tuile placée mais annule
 * le meeple, give_back, next_player et end_game. */
void engine_revert_partial_keeps_tile(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);

  /* Garde le dispatch tile_place uniquement */
  assert_int_equal(engine_revert(&engine, 1), SUCCESS);

  assert_int_equal(vector_size(&engine.dispatchs), 1);
  assert_non_null(*game_tile_at(&engine.game, 1, 0));
  /* current_hook pointe sur tile_place (hooks[0]) */
  assert_int_equal(engine.current_hook, 0);

  destroy_engine(&engine);
}

/* engine_revert(0) après un tour complet remet current_player à 0. */
void engine_revert_restores_player_index(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);

  /* next_player_fw a fait passer de 0 à 1 */
  assert_int_equal(engine.game.current_player, 1);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);
  assert_int_equal(engine.game.current_player, 0);

  destroy_engine(&engine);
}

/* meeple_place_bw restaure le compte de meeples du joueur. */
void engine_revert_restores_meeple_count(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  player_t*    player0 = &engine.game.players[0];
  unsigned int initial =
      ((meeple_count_t*)vector_nth(&player0->meeples_count, BASIC))->count;

  /* Pose FRFR en (1,0) : groupe B = ROAD (W-E), non terminée */
  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  action_t meeple                       = {0};
  meeple.type                           = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
  meeple.order.place_meeple.x           = 1;
  meeple.order.place_meeple.y           = 0;
  meeple.order.place_meeple.part_group  = LIBCARCASSONNE_TILE_PART_B;
  meeple.order.place_meeple.meeple_type = BASIC;
  assert_int_equal(dispatch_action(&engine, meeple), SUCCESS);

  /* Le count a diminué après placement */
  unsigned int after_place =
      ((meeple_count_t*)vector_nth(&player0->meeples_count, BASIC))->count;
  assert_true(after_place < initial);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  /* Restauré exactement */
  unsigned int after_revert =
      ((meeple_count_t*)vector_nth(&player0->meeples_count, BASIC))->count;
  assert_int_equal(after_revert, initial);

  destroy_engine(&engine);
}

/* Vérifie la taille du vecteur dispatchs aux différentes étapes. */
void engine_revert_dispatch_vector_size(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(vector_size(&engine.dispatchs), 1);

  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);
  assert_int_equal(vector_size(&engine.dispatchs), 5);

  assert_int_equal(engine_revert(&engine, 3), SUCCESS);
  assert_int_equal(vector_size(&engine.dispatchs), 3);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);
  assert_int_equal(vector_size(&engine.dispatchs), 0);

  destroy_engine(&engine);
}

/* Après revert(0), current_hook vaut 0 (prêt pour PLACE_TILE). */
void engine_revert_current_hook_zero_on_empty(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  /* Après dispatch tile : current_hook = 1 (meeple_place) */
  assert_int_equal(engine.current_hook, 1);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);
  assert_int_equal(engine.current_hook, 0);

  destroy_engine(&engine);
}

/* Deux tours joués, revert(5) annule le second tour et conserve le premier. */
void engine_revert_two_turns_to_epoch_five(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  /* Tour 1 (joueur 0) : FRFR en (1,0) — sud de la tuile de départ */
  const tile_t* frfr1 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr1);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr1, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);
  assert_int_equal(engine.game.current_player, 1);

  /* Tour 2 (joueur 1) : FRFR en (0,1) — est de la tuile de départ
   * CRFR E=ROAD, FRFR W=ROAD → compatible */
  const tile_t* frfr2 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr2);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr2, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);
  assert_int_equal(vector_size(&engine.dispatchs), 10);

  /* Retour à l'état après le premier tour */
  assert_int_equal(engine_revert(&engine, 5), SUCCESS);

  assert_int_equal(vector_size(&engine.dispatchs), 5);
  assert_non_null(*game_tile_at(&engine.game, 1, 0)); /* tour 1 conservé */
  assert_null(*game_tile_at(&engine.game, 0, 1));     /* tour 2 annulé */
  assert_int_equal(engine.game.current_player, 1);    /* joueur 1 attendu */

  destroy_engine(&engine);
}

/* end_game_bw restaure game.state à GAME_STATE_PLAYING après la fin de partie.
 * Avec 2 joueurs et max_turns=1, la fin est déclenchée après 1 round complet
 * (10 dispatches). */
void engine_revert_end_game_state_restored(void** state) {
  (void)state;
  options_t o      = revert_opts(2, 1);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  action_t mn = none_meeple_action();

  /* Tour joueur 0 : FRFR en (1,0) */
  const tile_t* frfr1 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr1);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr1, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, mn), SUCCESS);

  /* Tour joueur 1 : FRFR en (0,1) */
  const tile_t* frfr2 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr2);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(frfr2, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);
  assert_int_equal(dispatch_action(&engine, mn), SUCCESS);

  /* Après 1 round : end_game_fw a déclenché la fin de partie */
  assert_int_equal(engine.game.state, GAME_STATE_FINISHED);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);
  assert_int_equal(engine.game.state, GAME_STATE_PLAYING);

  destroy_engine(&engine);
}

/* Après un revert complet, dispatch_action fonctionne à nouveau normalement. */
void engine_revert_then_dispatch_works(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);
  action_t a = tile_action(frfr, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  assert_int_equal(dispatch_action(&engine, a), NO_PROGRESS);
  assert_int_equal(engine_revert(&engine, 0), SUCCESS);
  assert_null(*game_tile_at(&engine.game, 1, 0));

  /* Le moteur doit pouvoir redispatche la même action sans erreur */
  assert_int_equal(dispatch_action(&engine, a), NO_PROGRESS);
  assert_non_null(*game_tile_at(&engine.game, 1, 0));

  destroy_engine(&engine);
}

/*
 * Test d'intégration : jouer 1 round complet (3 joueurs), puis
 * engine_revert(0), et vérifier que l'état est identique à celui juste après
 * start_game.
 *
 * Tuiles FRFR (FIELD/ROAD/FIELD/ROAD), meeples sur des groupes FIELD côté libre
 * (jamais complétés) → give_back_meeples toujours no-op → scores inchangés.
 *   Joueur 0 : FRFR en ( 1, 0) NORTH — CRFR S=FIELD ↔ FRFR N=FIELD, meeple C
 *   Joueur 1 : FRFR en ( 0, 1) NORTH — CRFR E=ROAD  ↔ FRFR W=ROAD,  meeple A
 *   Joueur 2 : FRFR en ( 0,-1) NORTH — CRFR W=ROAD  ↔ FRFR E=ROAD,  meeple C
 */
void engine_revert_long_play_with_revert(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  /* --- Snapshot de l'état initial ---------------------------------------- */

  unsigned int init_deck_size = engine.game.deck.list.meta.size;

  unsigned int init_meeples_p0 =
      ((meeple_count_t*)vector_nth(&engine.game.players[0].meeples_count,
                                   BASIC))
          ->count;
  unsigned int init_meeples_p1 =
      ((meeple_count_t*)vector_nth(&engine.game.players[1].meeples_count,
                                   BASIC))
          ->count;
  unsigned int init_meeples_p2 =
      ((meeple_count_t*)vector_nth(&engine.game.players[2].meeples_count,
                                   BASIC))
          ->count;

  assert_int_equal(init_meeples_p0, 5);
  assert_int_equal(init_meeples_p1, 5);
  assert_int_equal(init_meeples_p2, 5);

  /* --- Tour du joueur 0 : FRFR en (1, 0) --------------------------------- */

  const tile_t* t0 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(t0);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(t0, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  action_t meeple0 = {.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE};
  meeple0.order.place_meeple.tile = *game_tile_at(&engine.game, 1, 0);
  meeple0.order.place_meeple.x    = 1;
  meeple0.order.place_meeple.y    = 0;
  meeple0.order.place_meeple.part_group =
      LIBCARCASSONNE_TILE_PART_C; /* FIELD sud, côté libre */
  meeple0.order.place_meeple.meeple_type = BASIC;
  assert_int_equal(dispatch_action(&engine, meeple0), SUCCESS);

  /* --- Tour du joueur 1 : FRFR en (0, 1) --------------------------------- */

  const tile_t* t1 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(t1);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(t1, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  action_t meeple1 = {.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE};
  meeple1.order.place_meeple.tile = *game_tile_at(&engine.game, 0, 1);
  meeple1.order.place_meeple.x    = 0;
  meeple1.order.place_meeple.y    = 1;
  meeple1.order.place_meeple.part_group =
      LIBCARCASSONNE_TILE_PART_A; /* FIELD nord, côté libre */
  meeple1.order.place_meeple.meeple_type = BASIC;
  assert_int_equal(dispatch_action(&engine, meeple1), SUCCESS);

  /* --- Tour du joueur 2 : FRFR en (0,-1) --------------------------------- */

  const tile_t* t2 = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(t2);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(t2, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  action_t meeple2 = {.type = LIBCARCASSONNE_ACTION_PLACE_MEEPLE};
  meeple2.order.place_meeple.tile = *game_tile_at(&engine.game, 0, -1);
  meeple2.order.place_meeple.x    = 0;
  meeple2.order.place_meeple.y    = -1;
  meeple2.order.place_meeple.part_group =
      LIBCARCASSONNE_TILE_PART_C; /* FIELD sud, côté libre */
  meeple2.order.place_meeple.meeple_type = BASIC;
  assert_int_equal(dispatch_action(&engine, meeple2), SUCCESS);

  /* --- Sanité post-jeu --------------------------------------------------- */

  assert_int_equal(vector_size(&engine.dispatchs), 15);
  assert_int_equal(engine.game.turn, 1);
  assert_int_equal(engine.game.current_player, 0);
  assert_non_null(*game_tile_at(&engine.game, 1, 0));
  assert_non_null(*game_tile_at(&engine.game, 0, 1));
  assert_non_null(*game_tile_at(&engine.game, 0, -1));

  /* --- Revert complet ----------------------------------------------------- */

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  /* --- Vérification : état identique à l'état initial -------------------- */

  /* Moteur */
  assert_int_equal(vector_size(&engine.dispatchs), 0);
  assert_int_equal(engine.current_hook, 0);

  /* Partie */
  assert_int_equal(engine.game.state, GAME_STATE_PLAYING);
  assert_int_equal(engine.game.turn, 0);
  assert_int_equal(engine.game.current_player, 0);

  /* Scores */
  assert_int_equal(engine.game.players[0].score, 0);
  assert_int_equal(engine.game.players[1].score, 0);
  assert_int_equal(engine.game.players[2].score, 0);

  /* Meeples restaurés */
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[0].meeples_count, BASIC))
                       ->count,
                   init_meeples_p0);
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[1].meeples_count, BASIC))
                       ->count,
                   init_meeples_p1);
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[2].meeples_count, BASIC))
                       ->count,
                   init_meeples_p2);

  /* Deck */
  assert_int_equal(engine.game.deck.list.meta.size, init_deck_size);

  /* Plateau */
  assert_null(*game_tile_at(&engine.game, 1, 0));
  assert_null(*game_tile_at(&engine.game, 0, 1));
  assert_null(*game_tile_at(&engine.game, 0, -1));
  assert_non_null(*game_tile_at(&engine.game, 0, 0)); /* tuile de départ */

  destroy_engine(&engine);
}

/*
 * Séquence mixte revert / jeu :
 *   1. Pose tuile A  →  revert(0)           (annule la pose)
 *   2. Pose tuile B  →  meeple NONE         (tour complet, joueur 0)
 *   3. Pose tuile C                          (début tour joueur 1, mid-turn)
 *   4. revert(0)                             (annule tout)
 *   Vérifier que l'état est identique à celui juste après start_game.
 */
void engine_revert_interleaved_revert_and_play(void** state) {
  (void)state;
  options_t o      = revert_opts(3, 0);
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  /* Snapshot initial */
  unsigned int init_deck_size = engine.game.deck.list.meta.size;
  unsigned int init_meeples_p0 =
      ((meeple_count_t*)vector_nth(&engine.game.players[0].meeples_count,
                                   BASIC))
          ->count;
  unsigned int init_meeples_p1 =
      ((meeple_count_t*)vector_nth(&engine.game.players[1].meeples_count,
                                   BASIC))
          ->count;
  unsigned int init_meeples_p2 =
      ((meeple_count_t*)vector_nth(&engine.game.players[2].meeples_count,
                                   BASIC))
          ->count;

  /* --- 1. Pose tuile A en (1,0), puis revert ----------------------------- */

  const tile_t* tile_a = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(tile_a);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(tile_a, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  assert_non_null(*game_tile_at(&engine.game, 1, 0));
  assert_int_equal(vector_size(&engine.dispatchs), 1);

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  assert_null(*game_tile_at(&engine.game, 1, 0));
  assert_int_equal(vector_size(&engine.dispatchs), 0);
  assert_int_equal(engine.current_hook, 0);

  /* --- 2. Pose tuile B en (0,1) + meeple NONE (tour complet joueur 0) --- */

  const tile_t* tile_b = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(tile_b);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(tile_b, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  assert_non_null(*game_tile_at(&engine.game, 0, 1));

  assert_int_equal(dispatch_action(&engine, none_meeple_action()), SUCCESS);

  assert_int_equal(vector_size(&engine.dispatchs), 5);
  assert_int_equal(engine.game.current_player, 1);

  /* --- 3. Pose tuile C en (0,-1) (début du tour joueur 1) ---------------- */

  const tile_t* tile_c = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(tile_c);
  assert_int_equal(
      dispatch_action(
          &engine,
          tile_action(tile_c, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH)),
      NO_PROGRESS);

  assert_non_null(*game_tile_at(&engine.game, 0, -1));
  assert_int_equal(vector_size(&engine.dispatchs), 6);

  /* --- 4. Revert complet -------------------------------------------------- */

  assert_int_equal(engine_revert(&engine, 0), SUCCESS);

  /* Moteur */
  assert_int_equal(vector_size(&engine.dispatchs), 0);
  assert_int_equal(engine.current_hook, 0);

  /* Partie */
  assert_int_equal(engine.game.state, GAME_STATE_PLAYING);
  assert_int_equal(engine.game.turn, 0);
  assert_int_equal(engine.game.current_player, 0);

  /* Scores */
  assert_int_equal(engine.game.players[0].score, 0);
  assert_int_equal(engine.game.players[1].score, 0);
  assert_int_equal(engine.game.players[2].score, 0);

  /* Meeples restaurés */
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[0].meeples_count, BASIC))
                       ->count,
                   init_meeples_p0);
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[1].meeples_count, BASIC))
                       ->count,
                   init_meeples_p1);
  assert_int_equal(((meeple_count_t*)vector_nth(
                        &engine.game.players[2].meeples_count, BASIC))
                       ->count,
                   init_meeples_p2);

  /* Deck */
  assert_int_equal(engine.game.deck.list.meta.size, init_deck_size);

  /* Plateau */
  assert_null(*game_tile_at(&engine.game, 1, 0));
  assert_null(*game_tile_at(&engine.game, 0, 1));
  assert_null(*game_tile_at(&engine.game, 0, -1));
  assert_non_null(*game_tile_at(&engine.game, 0, 0)); /* tuile de départ */

  destroy_engine(&engine);
}

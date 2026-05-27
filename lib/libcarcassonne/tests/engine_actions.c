#include <libcarcassonne/action.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/meeple.h>
#include <libcarcassonne/tests/tests.h>

// Utilise des options locales pour éviter la pollution de max_turns
static options_t engine_test_options(void) {
  static const extension_t*       ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};
  static const extension_vector_t ext         = {
      .meta = {.size = 1, .caps = 1, .data = &ptr_table}};
  options_t o = {.mode       = CARCASSONNE_MODE_CLI,
                 .players    = 3,
                 .seed       = 500,
                 .ai         = 0,
                 .max_turns  = 0,
                 .extensions = ext};
  return o;
}

void engine_wanted_action_is_place_tile_at_start(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  // Hook 0 = tile_place (priorité 4, PLACE_TILE)
  assert_int_equal(engine_wanted_action(&engine),
                   LIBCARCASSONNE_ACTION_PLACE_TILE);

  destroy_engine(&engine);
}

void engine_get_actions_not_empty_at_start(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  // tile_place_list_actions pioche une tuile et retourne les placements valides
  action_vector_t actions = engine_get_actions(&engine);
  assert_true(vector_size(&actions) > 0);

  // Toutes les actions proposées doivent être de type PLACE_TILE
  for (unsigned int i = 0; i < vector_size(&actions); i++) {
    action_t* a = vector_nth(&actions, i);
    assert_int_equal(a->type, LIBCARCASSONNE_ACTION_PLACE_TILE);
  }

  vector_free(&actions);
  destroy_engine(&engine);
}

void engine_wanted_action_is_place_meeple_after_tile(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  // Après start_game, la tuile de départ CRFR est en (0,0) orientation NORTH.
  // FRFR à (1,0) NORTH : face nord = FIELD, compatible avec face sud de CRFR =
  // FIELD.
  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  action_t action                     = {0};
  action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
  action.order.place_tile.tile        = frfr;
  action.order.place_tile.x           = 1;
  action.order.place_tile.y           = 0;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH;

  // dispatch_action exécute tile_place_fw, puis s'arrête sur meeple_place_fw
  // (action type différent) → retourne NO_PROGRESS
  assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);

  assert_int_equal(engine_wanted_action(&engine),
                   LIBCARCASSONNE_ACTION_PLACE_MEEPLE);

  destroy_engine(&engine);
}

void engine_get_actions_meeple_includes_none_option(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  action_t action                     = {0};
  action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
  action.order.place_tile.tile        = frfr;
  action.order.place_tile.x           = 1;
  action.order.place_tile.y           = 0;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH;
  assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);

  // En phase meeple, la dernière action est toujours "ne pas poser de meeple"
  action_vector_t meeple_actions = engine_get_actions(&engine);
  assert_true(vector_size(&meeple_actions) > 0);

  action_t* last =
      vector_nth(&meeple_actions, vector_size(&meeple_actions) - 1);
  assert_int_equal(last->order.place_meeple.meeple_type, NONE);

  vector_free(&meeple_actions);
  destroy_engine(&engine);
}

void engine_get_actions_only_none_when_no_meeples(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* frfr = deck_find_tile(&engine.game.deck, "FRFR", false);
  assert_non_null(frfr);

  action_t action                     = {0};
  action.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
  action.order.place_tile.tile        = frfr;
  action.order.place_tile.x           = 1;
  action.order.place_tile.y           = 0;
  action.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH;
  assert_int_equal(dispatch_action(&engine, action), NO_PROGRESS);

  // Vider tous les meeples du joueur courant
  player_t* current = game_get_current_player(&engine.game);
  for (unsigned int mt = 0; mt < vector_size(&current->meeples_count); mt++) {
    ((meeple_count_t*)vector_nth(&current->meeples_count, mt))->count = 0;
  }

  // Seule l'action NONE doit être proposée
  action_vector_t meeple_actions = engine_get_actions(&engine);
  assert_int_equal(vector_size(&meeple_actions), 1);

  action_t* only = vector_nth(&meeple_actions, 0);
  assert_int_equal(only->order.place_meeple.meeple_type, NONE);

  vector_free(&meeple_actions);
  destroy_engine(&engine);
}

/* Un meeple posé dans un groupe FIELD coin (open_slots=0, donc toujours
 * "complet") ne doit PAS être restitué en cours de partie.
 * Les champs Carcassonne ne sont scorés qu'en fin de partie.
 *
 * Scénario : CRFR placée en (0,1) est de la tuile de départ CRFR(0,0).
 *   - face W=ROAD compatible avec face E=ROAD de CRFR(0,0) ✓
 *   - groupe A = FIELD coins NW+NE (positions 0 et 2, aucune face)
 *     → open_slots=0 → placed_tile_group_complete() retourne true d'emblée
 * Sans le correctif, give_back_meeples_fw retire immédiatement le meeple. */
void engine_field_meeple_not_given_back_mid_game(void** state) {
  (void)state;
  options_t o      = engine_test_options();
  engine_t  engine = {0};
  assert_int_equal(create_engine(&engine, o), SUCCESS);
  assert_int_equal(start_game(&engine), SUCCESS);

  const tile_t* crfr = deck_find_tile(&engine.game.deck, "CRFR", false);
  assert_non_null(crfr);

  action_t tile_act                     = {0};
  tile_act.type                         = LIBCARCASSONNE_ACTION_PLACE_TILE;
  tile_act.order.place_tile.tile        = crfr;
  tile_act.order.place_tile.x           = 0;
  tile_act.order.place_tile.y           = 1;
  tile_act.order.place_tile.orientation = LIBCARCASSONNE_TILE_ORIENTATION_NORTH;
  assert_int_equal(dispatch_action(&engine, tile_act), NO_PROGRESS);

  player_t*    player = game_get_current_player(&engine.game);
  unsigned int before_count =
      ((meeple_count_t*)vector_nth(&player->meeples_count, BASIC))->count;

  action_t meeple_act                      = {0};
  meeple_act.type                          = LIBCARCASSONNE_ACTION_PLACE_MEEPLE;
  meeple_act.order.place_meeple.x          = 0;
  meeple_act.order.place_meeple.y          = 1;
  meeple_act.order.place_meeple.part_group = A;
  meeple_act.order.place_meeple.meeple_type = BASIC;
  assert_int_equal(dispatch_action(&engine, meeple_act), SUCCESS);

  /* Le meeple doit encore être dans le groupe FIELD après le tour complet */
  placed_tile_t** p = game_tile_at(&engine.game, 0, 1);
  assert_non_null(*p);
  assert_non_null((*p)->groups[A]->meeple);

  /* Le compteur doit avoir diminué de 1 (meeple posé, non restitué) */
  unsigned int after_count = ((meeple_count_t*)vector_nth(
                                  &engine.game.players[0].meeples_count, BASIC))
                                 ->count;
  assert_int_equal(after_count, before_count - 1);

  destroy_engine(&engine);
}

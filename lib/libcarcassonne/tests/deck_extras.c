#include <libcarcassonne/deck.h>
#include <libcarcassonne/tests/tests.h>

void deck_pick_removes_tile(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);
  int initial_size = list_size(&deck.list);

  const tile_t* tile = deck_pick(&deck);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(list_size(&deck.list), initial_size - 1);

  free_deck(deck);
}

void deck_pick_on_empty_returns_null(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);

  while (deck_pick(&deck) != NULL) {
    /* continue */
  }

  const tile_t* tile = deck_pick(&deck);
  assert_ptr_equal(tile, NULL);

  free_deck(deck);
}

void deck_defausser_adds_tile_back(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);
  int initial_size = list_size(&deck.list);

  const tile_t* tile = deck_pick(&deck);
  assert_ptr_not_equal(tile, NULL);

  deck_defausser(&deck, tile);
  assert_int_equal(list_size(&deck.list), initial_size);

  free_deck(deck);
}

void deck_defausser_random_position(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);

  const tile_t* tile1 = deck_pick(&deck);
  const tile_t* tile2 = deck_pick(&deck);

  deck_defausser(&deck, tile1);
  deck_defausser(&deck, tile2);

  assert_int_equal(list_size(&deck.list), LIBCARCASSONNE_TILES_COUNT);

  free_deck(deck);
}

void deck_find_tile_removes_match(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);
  int initial_size = list_size(&deck.list);

  const tile_t* tile = deck_find_tile(&deck, "FCFC", false);
  assert_ptr_not_equal(tile, NULL);
  assert_int_equal(list_size(&deck.list), initial_size - 1);

  free_deck(deck);
}

void deck_find_tile_returns_null_if_not_found(void** state) {
  (void)state;

  deck_t deck = create_deck(42, &options.extensions);

  const tile_t* tile = deck_find_tile(&deck, "XXXX", false);
  assert_ptr_equal(tile, NULL);

  free_deck(deck);
}

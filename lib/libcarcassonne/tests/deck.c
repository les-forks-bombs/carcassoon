#include <libcarcassonne/deck.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>

/* Vérifie l'instanciation d'un deck */
void deck_builds(void** state) {
  (void)state;

  deck_t deck = create_deck(0, &options.extensions);
  free_deck(deck);
}

/* Vérifie que le deck est consistant quand on a les memes seed */
void deck_seed_consistent(void** state) {
  (void)state;

  deck_t deck1 = create_deck(0, &options.extensions);
  deck_t deck2 = create_deck(0, &options.extensions);
  deck_t deck3 = create_deck(100, &options.extensions);

  // on skip le premier car c'est toujours le meme (tile de départ)
  const tile_t* r = deck_pick(&deck1);
  assert_ptr_equal(deck_pick(&deck2), r);
  assert_ptr_equal(deck_pick(&deck3), r);

  int i = 1;  // on mets le compteur a 1 car on a fait un premier appel
  while (list_head(&deck1.list) != NULL) {
    r = deck_pick(&deck1);

    // on vérifie que deux générateurs doivent être égals
    assert_ptr_equal(r, deck_pick(&deck2));

    // au début, on vérifie que nos générateurs diffèrent bien
    if (i < 1) assert_ptr_equal(r, deck_pick(&deck3));

    i++;
  }

  r = deck_pick(&deck1);
  assert_ptr_equal(r, deck_pick(&deck2));
  assert_null(r);

  // on vérifie qu'on a bien les 72 tiles
  assert_int_equal(i, 72);

  free_deck(deck1);
  free_deck(deck2);
  free_deck(deck3);
}

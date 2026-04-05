#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>

#include <libcarcassonne/deck.h>
#include <libcarcassonne/tests/tests.h>

/* Vérifie l'instanciation d'un deck */
void deck_builds(void **state)
{
    deck_t deck = create_deck(0);
    free_deck(deck);
}

/* Vérifie que le deck est consistant quand on a les memes seed */
void deck_seed_consistent(void **state)
{
    deck_t deck1 = create_deck(0);
    deck_t deck2 = create_deck(0);
    deck_t deck3 = create_deck(100);

    // we skip the first pick because it's always the starting one
    
    tile_t *r = deck_pick(&deck1);
    assert_string_equal(deck_pick(&deck2)->name, r->name);
    assert_string_equal(deck_pick(&deck3)->name, r->name);

    int i = 1; // set to one because we skipped the first one
    while (deck1.list.head != NULL)
    {
        r = deck_pick(&deck1);

        // on vérifie que deux générateurs doivent être égals
        assert_string_equal(r->name, deck_pick(&deck2)->name);

        // au début, on vérifie que nos générateurs diffèrent bien
        if (i < 1)
            assert_string_not_equal(r->name, deck_pick(&deck3)->name);

        i++;
    }

    r = deck_pick(&deck1);
    assert_ptr_equal(r, deck_pick(&deck2));
    assert_null(r);

    assert_int_equal(i, 72);

    free_deck(deck1);
    free_deck(deck2);
    free_deck(deck3);
}

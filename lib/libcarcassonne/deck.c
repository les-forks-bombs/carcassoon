#include <libcarcassonne/deck.h>
#include <libcarcassonne/deck_composition.h>
#include <stdio.h>

deck_t create_deck(
    int seed
) {
    deck_t deck = {
        .seed = seed,
        .size = 0,
    };

    for (
        int i = 0;
        i < sizeof(tiles) / sizeof(tiles[0]);
        i++
    ) {
        deck_tile_t element = tiles[i];
        printf("Tile %s: %d\n", element.tile.name, element.remaining);
        
        for (
            int j = 0;
            j < element.remaining;
            j++
        ) {
            // printf("Instanciating (%d): %s %d/%d\n", deck.size++, element.tile.name, j + 1, element.remaining);
        }
    }

}


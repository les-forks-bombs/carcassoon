#include <libcarcassonne/libcarcassonne.h>
#include <stdio.h>

int main(void) {
    game_t game;

    create_game(&game, 3, 0, 500, 0);
    
    tile_t* tile = deck_pick(&game.deck);

    game_place_tile(&game, tile, 0, 0);
    game_place_tile(&game, tile, 1, 0);
    game_place_tile(&game, tile, 0, 1);
    game_place_tile(&game, tile, -1, 0);
    game_place_tile(&game, tile, 0, -1);

    game_print_map(&game);

    destroy_game(&game);
}

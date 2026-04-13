#include <libcarcassonne/libcarcassonne.h>
#include <stdio.h>

int main(void) {
  game_t game;

  create_game(&game, 3, 0, 500, 0);

  tile_t* tile = deck_pick(&game.deck);
  deck_defausser(&game.deck, tile);

  game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
  game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
  game_place_tile(&game, tile, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
  game_place_tile(&game, tile, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);
  game_place_tile(&game, tile, 0, -1, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  game_print_map(&game);

  destroy_game(&game);
}

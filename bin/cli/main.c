#include <libcarcassonne/libcarcassonne.h>
#include <stdio.h>
#include <string.h>

tile_t* find_tile(game_t* game, char* family) {
  deck_element_t* curr = game->deck.list.head;
  while (strcmp(curr->tile->family, family) != 0) {
    curr = curr->next;
  }

  return curr->tile;
}

int main(void) {
  game_t game;
  create_game(&game, 3, 0, 500, 0);

  tile_t* tile = deck_pick(&game.deck);
  game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  tile = find_tile(&game, "CFCF");

  // en bas
  printf("%d\n", game_place_tile(&game, tile, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST));

  // en haut
  printf("%d\n", game_place_tile(&game, tile, -1, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH));

  game_print_detail(&game, 0, 0, 3);

  destroy_game(&game);
}

#include <libcarcassonne/game.h>
#include <stdio.h>
#include <string.h>
#include "libcarcassonne/options.h"

tile_t* find_tile(game_t* game, char* family) {
  deck_element_t* curr = game->deck.list.head;
  while (strcmp(curr->tile->family, family) != 0) {
    curr = curr->next;
  }

  return curr->tile;
}

int main(void) {
  game_t game;
  options_t config = {
      .mode      = CARCASSONNE_MODE_CLI,
      .players   = 3,
      .seed      = 500,
      .ai        = 0,
      .max_turns = 0,
  };
  create_game(&game, config);

  tile_t* tile = deck_pick(&game.deck);
  game_place_tile(&game, tile, 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  tile = find_tile(&game, "CFCF");

  // en bas
  printf("%d\n", game_place_tile(&game, tile, 1, 0,
                                 LIBCARCASSONNE_TILE_ORIENTATION_WEST));

  // en haut
  printf("%d\n", game_place_tile(&game, tile, -1, 0,
                                 LIBCARCASSONNE_TILE_ORIENTATION_NORTH));

  destroy_game(&game);
}

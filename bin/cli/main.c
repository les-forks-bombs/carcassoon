#include <libcarcassonne/libcarcassonne.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  game_t game;

  create_game(&game, 3, 0, 500, 0);

  // play CCCC at 0,0
  game_place_tile(&game,  &tiles[0], 0, 0, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  tile_t t = {
      .amount = 1,
      .blason = 1,
      .family = "FCFC",
      .parts = {
        5, 5, 5,
        5, 5, 5,
        5, 5, 5,
      },
      .parts_groups = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
      }
  };

  game_place_tile(&game,  &t, 1, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST);
  game_place_tile(&game,  &tiles[0], 2, 0, LIBCARCASSONNE_TILE_ORIENTATION_WEST);
  game_place_tile(&game,  &t, 0, 1, LIBCARCASSONNE_TILE_ORIENTATION_EAST);
  game_place_tile(&game,  &tiles[0], 0, 2, LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  game_print_detail(&game, 0, 0, 3);

  destroy_game(&game);
}

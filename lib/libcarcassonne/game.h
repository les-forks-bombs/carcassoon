#ifndef H_LIBCARCASSONNE_GAME
#define H_LIBCARCASSONNE_GAME

#include <libcarcassonne/consts.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>

typedef struct game
{
    player_t *current_player;
    player_t players[LIBCARCASSONNE_MAX_PLAYERS];
    int nb_players;

    int turn;
    int max_turn;

    deck_t deck;

    tile_t map[LIBCARCASSONNE_TILES_COUNT *
               LIBCARCASSONNE_TILES_COUNT * 2];

} game_t;

game_t create_game();
void destroy_game(game_t game);

#endif
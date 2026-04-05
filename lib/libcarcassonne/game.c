#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libcarcassonne/consts.h>

game_t create_game(int nb_players, int nb_ai, int seed, int max_turns)
{
    assert(nb_players >= 2 && nb_players <= 5);
    assert(nb_ai >= 0 && nb_ai <= nb_players);

    game_t game = {
        .deck = create_deck(seed),
        .nb_players = nb_players,
        .turn = 0,
        .max_turn = max_turns,

    };

    for (int i = 0; i < nb_players; i++)
    {
        player_t p;
        if (i < nb_players - nb_ai)
        {
            p = create_player(LIBCARCASSONNE_PLAYER_HUMAN);
        }
        else
        {
            p = create_player(LIBCARCASSONNE_PLAYER_AI);
        }
        game.players[i] = p;
    }

    game.current_player = &game.players[0];

    return game;
}

void destroy_game(game_t game)
{

    game.current_player = NULL;

    free_deck(game.deck);

    game.nb_players = 0;
    game.turn = 0;
    game.max_turn = 0;
}

bool place_tile(game_t *game, tile_t tile, int x,int y){
    int zero = LIBCARCASSONNE_TILES_COUNT-1;
    if (x<=-LIBCARCASSONNE_TILES_COUNT ||
        y<=-LIBCARCASSONNE_TILES_COUNT ||
        x>=LIBCARCASSONNE_TILES_COUNT ||
        y>=LIBCARCASSONNE_TILES_COUNT){
        return false;
    }
    if (game->map[calc_index(zero+y,zero+x)]==NULL){
        game->map[calc_index(zero+y,zero+x)]=tile;
        return true;
    }
    else {
        return false;
    }
}

int calc_index(int y,int x){
    int size = LIBCARCASSONNE_TILES_COUNT*2-1;
    return y*size+x;
}

void print_map(game_t game){
    for(int i=0;i<LIBCARCASSONNE_TILES_COUNT*2-1;i++){
        for(int j=0;j<LIBCARCASSONNE_TILES_COUNT*2-1;j++){
            if (game.map[calc_index(i,j)]==NULL){
                printf(".");
            }
            else {
                printf("#");
            }
        }
        printf("\n");
    }
}

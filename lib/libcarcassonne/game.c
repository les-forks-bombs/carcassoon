#include "game.h"

#include "assert.h"
#include <stdlib.h>

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
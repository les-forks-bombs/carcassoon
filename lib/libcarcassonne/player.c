#include "player.h"

player_t create_player(player_type type)
{
    player_t player = {
        .player_type = type,
        .score = 0

    };

    return player;
}

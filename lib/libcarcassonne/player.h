#pragma once

#include <stdbool.h>

#include "libcarcassonne/enums.h"
#include "libcarcassonne/forward.h"

struct player {
  unsigned int          id;
  player_type_t         player_type;
  unsigned int          score;
  meeple_count_vector_t meeples_count;
  meeple_vector_t       meeples;
};

player_t create_player(player_type_t type, meeple_count_vector_t* meeples_count,
                       unsigned int id);
void     free_player(player_t* player);

bool player_has_meeple_to_place(player_t* player);
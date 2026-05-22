#pragma once

#include <libcarcassonne/meeple.h>
#include <stdbool.h>

#include "libcarcassonne/forward.h"

enum player_type {
  LIBCARCASSONNE_PLAYER_AI    = 0,
  LIBCARCASSONNE_PLAYER_HUMAN = 1
};

typedef enum player_type player_type_t;
struct player {
  player_type_t         player_type;
  unsigned int          score;
  meeple_count_vector_t meeples_count;
  meeple_vector_t       meeples;
};

player_t create_player(player_type_t          type,
                       meeple_count_vector_t* meeples_count);
void     free_player(player_t* player);

bool player_has_meeple_to_place(player_t* player);
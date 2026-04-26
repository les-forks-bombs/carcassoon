#pragma once

#include <libcarcassonne/meeple.h>

enum player_type {
  LIBCARCASSONNE_PLAYER_AI    = 0,
  LIBCARCASSONNE_PLAYER_HUMAN = 1
};

typedef enum player_type player_type_t;

typedef struct player {
  player_type_t       player_type;
  int                 score;
  meeple_count_list_t meeples_count;
  meeple_list_t       meeples;
} player_t;

player_t create_player(player_type_t type, meeple_count_list_t *meeples_count);
void free_player(player_t *player);

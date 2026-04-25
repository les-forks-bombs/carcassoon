#pragma once

#include <libcarcassonne/meeple.h>
#include <libutils/vector.h>

enum player_type {
  LIBCARCASSONNE_PLAYER_AI    = 0,
  LIBCARCASSONNE_PLAYER_HUMAN = 1
};

typedef Vector(struct meeple*) meeple_list_t;

typedef enum player_type player_type_t;

typedef struct player {
  player_type_t player_type;
  int           score;
  int           nb_meeples;
  meeple_list_t meeples;
} player_t;

player_t create_player(player_type_t type);

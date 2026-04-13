#ifndef H_LIBCARCASSONNE_PLAYER
#define H_LIBCARCASSONNE_PLAYER

#define LIBCARCASSONNE_PLAYER_AI    0
#define LIBCARCASSONNE_PLAYER_HUMAN 1

typedef char player_type;

typedef struct player {
  player_type player_type;
  int         score;
} player_t;

player_t create_player(player_type type);

#endif

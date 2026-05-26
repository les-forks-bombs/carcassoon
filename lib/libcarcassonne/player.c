#include <libcarcassonne/player.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libcarcassonne/forward.h"
#include "libcarcassonne/meeple.h"
#include "libutils/vector.h"

player_t create_player(player_type_t          type,
                       meeple_count_vector_t *meeples_count, unsigned int id) {
  player_t player;
  memset(&player, 0, sizeof(player_t));
  player.player_type = type;
  player.id = id;

  for (unsigned int i = 0; i < vector_size(meeples_count); i++) {
    meeple_count_t *current = vector_nth(meeples_count, i);
    vector_append(&player.meeples_count, current);
  }

  return player;
}

void free_player(player_t *player) {
  if (player == NULL) {
    return;
  }
  for (size_t i = 0; i < vector_size(&player->meeples); i++) {
    meeple_t *meeple = *vector_nth(&player->meeples, i);
    free(meeple);
  }

  vector_free(&player->meeples);
  vector_free(&player->meeples_count);
}

bool player_has_meeple_to_place(player_t *player) {
  meeple_count_t *meeple_count;
  for (unsigned int i = 0; i < vector_size(&player->meeples_count); i++) {
    meeple_count = vector_nth(&player->meeples_count, i);
    if (meeple_count->count > 0) {
      return true;
    }
  }

  return false;
}

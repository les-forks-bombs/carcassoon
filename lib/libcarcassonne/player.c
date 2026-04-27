#include <libcarcassonne/player.h>
#include <stdlib.h>
#include <time.h>

#include "libcarcassonne/forward.h"
#include "libutils/vector.h"

player_t create_player(player_type_t type, meeple_count_list_t *meeples_count) {
  meeple_list_t *meeples = calloc(1, sizeof(meeple_list_t));

  meeple_count_list_t *player_meeples_count =
      calloc(1, sizeof(meeple_count_list_t));
  for (unsigned int i = 0; i < meeples_count->meta.size; i++) {
    meeple_count_t *current = vector_nth(meeples_count, i);

    meeple_count_t meeple_count = {.count=current->count,.meeple_type=current->meeple_type};

    vector_append(player_meeples_count, &meeple_count);
  }

  player_t player = {.player_type   = type,
                     .score         = 0,
                     .meeples_count = player_meeples_count,
                     .meeples       = meeples};

  return player;
}

void free_player(player_t *player) {
  //vector_free(player->meeples);
  vector_free(player->meeples_count);

  free(player->meeples);
  free(player->meeples_count);
}

#include <libcarcassonne/player.h>

player_t create_player(player_type_t type, meeple_count_list_t *meeples_count) {
  meeple_list_t *meeples;
  vector_alloc(meeples, 1);

  meeple_count_list_t *player_meeples_count;
  for (int i = 0; i < meeples_count->meta.size; i++) {
    meeple_count_t *current = &meeples_count->meta.data[i];

    meeple_count_t *meeple_count = malloc(sizeof(meeple_count_t));

    meeple_count->count       = current->count;
    meeple_count->meeple_type = current->meeple_type;

    vector_append(player_meeples_count, meeple_count);
  }

  player_t player = {.player_type   = type,
                     .score         = 0,
                     .meeples_count = player_meeples_count,
                     .meeples       = meeples};

  return player;
}

void free_player(player_t *player) {
  for (int i = 0; i < player->meeples->meta.size; i++) {
    meeple_t *meeple = (meeple_t *)&player->meeples->meta.data[i];
    free(meeple);
  }

  for (int i = 0; i < player->meeples_count->meta.size; i++) {
    meeple_count_t *meeple_count =
        (meeple_count_t *)&player->meeples_count->meta.data[i];
    free(meeple_count);
  }
}

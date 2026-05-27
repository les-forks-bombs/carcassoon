#include <assert.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libcarcassonne/forward.h"
#include "libcarcassonne/meeple.h"
#include "libcarcassonne/player.h"
#include "libutils/lc.h"

return_code_t create_game(game_t *game, options_t *options) {
  if (game == NULL) {
    return ERROR;
  }

  if (validate_options(options) != NULL) {
    return ERROR;
  }

  game->current_player = 0;
  game->deck           = create_deck(options->seed, &options->extensions);
  game->options        = options;
  game->state          = GAME_STATE_NOT_STARTED;
  game->turns_limit    = options->max_turns;

  game->open_tiles.meta.head = NULL;
  game->open_tiles.meta.tail = NULL;
  game->open_tiles.meta.size = 0;

  // todo: considérer les extensions pour calculer la taille max du tableau
  size_t largeur = LIBCARCASSONNE_TILES_COUNT + LIBCARCASSONNE_TILES_COUNT - 1;

  game->map =
      (placed_tile_t **)calloc(largeur * largeur, sizeof(placed_tile_t *));

  meeple_count_vector_t meeples_count = {0};

  meeples_count.meta.data = NULL;
  vector_alloc(&meeples_count, 3);
  meeples_count.meta.size = 0;

  meeple_count_t basic_meeple_count = {.count = 0, .meeple_type = BASIC};
  vector_append(&meeples_count, &basic_meeple_count);

  meeple_count_t large_meeple_count = {.count = 0, .meeple_type = LARGE};
  vector_append(&meeples_count, &large_meeple_count);

  meeple_count_t abbot_meeple_count = {.count = 0, .meeple_type = ABBOT};
  vector_append(&meeples_count, &abbot_meeple_count);

  for (unsigned int i = 0; i < vector_size(&options->extensions); i++) {
    extension_t          *extension = *vector_nth(&options->extensions, i);
    meeple_count_vector_t ext_meeple_count_list = extension->meeples_count;
    for (unsigned int j = 0; j < vector_size(&ext_meeple_count_list); j++) {
      meeple_count_t *ext_meeple_count = vector_nth(&ext_meeple_count_list, j);

      meeple_count_t *meeple_count =
          vector_nth(&meeples_count, ext_meeple_count->meeple_type);

      meeple_count->count += ext_meeple_count->count;
    }
  }

  // on instancie les joueurs
  for (unsigned int i = 0; i < game->options->players; i++) {
    game->players[i] =
        create_player(i > game->options->ai ? LIBCARCASSONNE_PLAYER_HUMAN
                                            : LIBCARCASSONNE_PLAYER_AI,
                      &meeples_count, i);
  }

  vector_free(&meeples_count);

  return SUCCESS;
}

void destroy_game(game_t *game) {
  if (game == NULL) {
    return;
  }

  // Cleanup des structures dans la map (les placed_tile_t)
  for (int i = -LIBCARCASSONNE_TILES_COUNT + 1; i < LIBCARCASSONNE_TILES_COUNT;
       i++) {
    for (int j = -LIBCARCASSONNE_TILES_COUNT + 1;
         j < LIBCARCASSONNE_TILES_COUNT; j++) {
      placed_tile_t **tile = game_tile_at(game, i, j);
      if (*tile != NULL) {
        placed_tile_destroy(*tile);
        free(*tile);
      }
    }
  }

  for (unsigned int i = 0; i < game->options->players; i++) {
    free_player(&game->players[i]);
  }

  list_free(&game->open_tiles);

  free((void *)game->map);
  free_deck(game->deck);
}

placed_tile_t **game_tile_at(game_t *game, int colonne, int ligne) {
  if (colonne <= -LIBCARCASSONNE_TILES_COUNT ||
      ligne <= -LIBCARCASSONNE_TILES_COUNT ||
      colonne >= LIBCARCASSONNE_TILES_COUNT ||
      ligne >= LIBCARCASSONNE_TILES_COUNT) {
    return NULL;  // Out of bounds
  }

  int zero = LIBCARCASSONNE_TILES_COUNT - 1;
  int size = (LIBCARCASSONNE_TILES_COUNT * 2) - 1;

  int index = ((colonne + zero) * size) + (ligne + zero);

  return &game->map[index];
}

return_code_t game_place_tile(game_t *game, const tile_t *tile, int x, int y,
                              tile_orientation_t orientation) {
  if (game == NULL || tile == NULL) {
    return ERROR;
  }

  placed_tile_t **tile_ref = game_tile_at(game, x, y);

  if (tile_ref == NULL) {
    return OUT_OF_BOUNDS;  // Out of bounds
  }

  if (*tile_ref == NULL) {
    if (!game_is_tile_placeable(game, tile, x, y, orientation)) {
      return INVALID_PLACEMENT;
    }

    placed_tile_t *placed_tile = calloc(1, sizeof(placed_tile_t));
    return_code_t  code =
        placed_tile_create(placed_tile, tile, orientation, x, y);

    if (code != SUCCESS) {
      return code;
    }

    *tile_ref = placed_tile;

    if (game_is_place_open(game, x, y)) {
      list_append(&game->open_tiles, &placed_tile);
    }

    if (!game_is_place_open(game, x + 1, y)) {  // Tuile du bas
      list_remove_value(&game->open_tiles, &*game_tile_at(game, x + 1, y));
    }

    if (!game_is_place_open(game, x - 1, y)) {  // Tuile du haut
      list_remove_value(&game->open_tiles, &*game_tile_at(game, x - 1, y));
    }

    if (!game_is_place_open(game, x, y + 1)) {  // Tuile de droite

      list_remove_value(&game->open_tiles, &*game_tile_at(game, x, y + 1));
    }

    if (!game_is_place_open(game, x, y - 1)) {  // Tuile de gauche
      list_remove_value(&game->open_tiles, &*game_tile_at(game, x, y - 1));
    }

    for (tile_orientation_t orientation = 0; orientation < 4; orientation++) {
      placed_tile_t **neighbor;

      switch (orientation) {
        case LIBCARCASSONNE_TILE_ORIENTATION_NORTH:
          // BAS
          neighbor = game_tile_at(game, x - 1, y);
          break;
        // HAUT
        case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH:
          neighbor = game_tile_at(game, x + 1, y);
          break;
        // DROITE
        case LIBCARCASSONNE_TILE_ORIENTATION_EAST:
          neighbor = game_tile_at(game, x, y + 1);
          break;
        // GAUCHE
        case LIBCARCASSONNE_TILE_ORIENTATION_WEST:
          neighbor = game_tile_at(game, x, y - 1);
          break;
      }

      // on se marque comme étant voisin de notre voisin, et réciproquement
      if (neighbor != NULL && *neighbor != NULL) {
        placed_tile_t   *rneighbor    = *neighbor;
        static const int values[4][3] = {
            {0, 1, 2}, {2, 5, 8}, {8, 7, 6}, {6, 3, 0}};

        unsigned int oindex = (orientation + placed_tile->orientation) % 4;
        unsigned int nindex =
            (tile_orientation_invert(orientation) + rneighbor->orientation) % 4;

        // pour chaque sous tile de la face
        for (int i = 0; i < 3; i++) {
          int ngroup = rneighbor->parent->parts_groups[values[nindex][i]];
          int ogroup = placed_tile->parent->parts_groups[values[oindex][i]];

          if (rneighbor->parent->parts[values[nindex][i]] ==
              placed_tile->parent->parts[values[oindex][i]]) {
            if (placed_tile->parent->parts[values[oindex][i]] !=
                    LIBCARCASSONNE_TILE_PART_FIELD &&
                placed_tile->parent->parts[values[oindex][i]] !=
                    LIBCARCASSONNE_TILE_PART_WALL &&
                placed_tile_group_link(rneighbor->groups[ngroup],
                                       placed_tile->groups[ogroup])) {
              placed_tile->groups[ogroup]->open_slots--;
              rneighbor->groups[ngroup]->open_slots--;
            }
          }
        }
      }
    }

    return SUCCESS;  // Placed
  }
  return NOT_FREE;  // not Free
}

return_code_t game_remove_tile(game_t *game, int x, int y) {
  placed_tile_t **tile_ref = game_tile_at(game, x, y);

  if (tile_ref == NULL) return OUT_OF_BOUNDS;
  if (*tile_ref == NULL) return NO_TILE;

  placed_tile_t *placed_tile = *tile_ref;

  bool visite[9];
  memset(&visite, 0, sizeof(visite));

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!visite[group]) {
      meeple_t *meeple = placed_tile->groups[group]->meeple;
      // pour chaque groupe, si un meeple est placé
      if (meeple != NULL) {
        return REMOVE_TILE_NOT_VOID;
      }
      placed_tile_group_t *pgroup = placed_tile->groups[group];

      // on le déconnecte de tous ses voisins
      for (unsigned int i = 0; i < vector_size(&pgroup->neighbors); i++) {
        placed_tile_group_t *neigh = *vector_nth(&pgroup->neighbors, i);
        vector_remove_value(&neigh->neighbors, &pgroup);
      }
    }
    visite[group] = true;
  }

  placed_tile_destroy(*tile_ref);
  free(*tile_ref);
  *tile_ref = NULL;

  return SUCCESS;
}

return_code_t game_place_meeple(game_t *game, int x, int y, int group,
                                meeple_type_t meeple_type, player_t *player) {
  if (game == NULL || player == NULL) {
    return ERROR;
  }

  if (meeple_type == NONE) {
    return SUCCESS;
  }

  placed_tile_t **tile_ref = game_tile_at(game, x, y);

  if (tile_ref == NULL) {
    return OUT_OF_BOUNDS;  // Out of bounds
  }

  if (*tile_ref != NULL) {
    placed_tile_group_t *group_ref = (*tile_ref)->groups[group];

    if (group_ref->meeple == NULL) {
      meeple_t *meeple    = calloc(1, sizeof(meeple_t));
      meeple->group       = group;
      meeple->group_node  = group_ref;
      meeple->meeple_type = meeple_type;

      group_ref->meeple         = meeple;
      group_ref->meeple->player = player;

      vector_append(&player->meeples, &meeple);
      (vector_nth(&player->meeples_count, meeple->meeple_type))->count--;

    } else {
      return ALREADY_ALLOCATED;
    }
    return SUCCESS;
  }
  return NO_TILE;
}

return_code_t game_remove_meeple(game_t *game, int x, int y, int part_group) {
  if (game == NULL) {
    return ERROR;
  }

  placed_tile_t **tile_ref = game_tile_at(game, x, y);

  if (tile_ref == NULL) {
    return OUT_OF_BOUNDS;  // Out of bounds
  }

  if (*tile_ref != NULL) {
    placed_tile_group_t *group_ref = (*tile_ref)->groups[part_group];

    if (group_ref->meeple != NULL) {
      meeple_t *meeple = group_ref->meeple;

      (vector_nth(&meeple->player->meeples_count, meeple->meeple_type))
          ->count++;

      vector_remove_value(&meeple->player->meeples, &meeple);
      free(group_ref->meeple);
      group_ref->meeple = NULL;
    }

    return SUCCESS;
  }
  return NO_TILE;
}

bool game_is_tile_placeable(game_t *game, const tile_t *tile, int x, int y,
                            tile_orientation_t orientation) {
  if (game == NULL || tile == NULL) return false;

  if (x == 0 && y == 0) {
    return true;
  }

  placed_tile_t **up_tile    = game_tile_at(game, x - 1, y);
  placed_tile_t **down_tile  = game_tile_at(game, x + 1, y);
  placed_tile_t **left_tile  = game_tile_at(game, x, y - 1);
  placed_tile_t **right_tile = game_tile_at(game, x, y + 1);

  if ((up_tile == NULL || *up_tile == NULL) &&
      (down_tile == NULL || *down_tile == NULL) &&
      (left_tile == NULL || *left_tile == NULL) &&
      (right_tile == NULL || *right_tile == NULL)) {
    return false;
  }

  if (*up_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH) !=
          tile_get_family_face((*up_tile)->parent, (*up_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_SOUTH)) {
    return false;
  }

  if (down_tile != NULL && *down_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH) !=
          tile_get_family_face((*down_tile)->parent, (*down_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_NORTH)) {
    return false;
  }

  if (left_tile != NULL && *left_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST) !=
          tile_get_family_face((*left_tile)->parent, (*left_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_EAST)) {
    return false;
  }

  if (right_tile != NULL && *right_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST) !=
          tile_get_family_face((*right_tile)->parent,
                               (*right_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_WEST)) {
    return false;
  }

  return true;
}

bool game_is_place_open(game_t *game, int x, int y) {
  if (!game_is_space_available(game, x + 1, y) &&
      !game_is_space_available(game, x - 1, y) &&
      !game_is_space_available(game, x, y + 1) &&
      !game_is_space_available(game, x, y - 1)) {
    return false;
  }

  return true;
}

bool is_game_finished(game_t *game) {
  return list_size(&game->deck.list) == 0 ||
         (game->turns_limit != 0 && game->turn >= game->turns_limit);
}

return_code_t game_end_player_turn(game_t *game) {
  if (game == NULL) {
    return NULL_POINTER;
  }

  if (game->current_player + 1 >= game->options->players) {
    return NO_MORE_PLAYER;
  }

  game->current_player++;

  return SUCCESS;
}

return_code_t game_end_round(game_t *game) {
  if (game == NULL) {
    return NULL_POINTER;
  }

  if (game->current_player != game->options->players - 1) {
    return PLAYER_NOT_CALLED;
  }

  game->current_player = 0;
  game->turn++;

  return SUCCESS;
}

player_t *game_get_current_player(game_t *game) {
  return &game->players[game->current_player];
}

vector2d_vector_t game_get_available_space(game_t *game) {
  vector2d_vector_t vec = {0};
  vector_alloc(&vec, list_size(&game->open_tiles));
  vec.meta.size = 0;

  list_node_t *current = list_head(&game->open_tiles);

  while (current != NULL) {
    placed_tile_t *current_tile = *list_value(&game->open_tiles, current);

    if (game_is_space_available(game, current_tile->x + 1, current_tile->y)) {
      vector2d_t spot = {.x = current_tile->x + 1, .y = current_tile->y};
      if (!vector_contains(&vec, &spot)) {
        vector_append(&vec, &spot);
      }
    }

    if (game_is_space_available(game, current_tile->x - 1, current_tile->y)) {
      vector2d_t spot = {.x = current_tile->x - 1, .y = current_tile->y};
      if (!vector_contains(&vec, &spot)) {
        vector_append(&vec, &spot);
      }
    }

    if (game_is_space_available(game, current_tile->x, current_tile->y + 1)) {
      vector2d_t spot = {.x = current_tile->x, .y = current_tile->y + 1};
      if (!vector_contains(&vec, &spot)) {
        vector_append(&vec, &spot);
      }
    }

    if (game_is_space_available(game, current_tile->x, current_tile->y - 1)) {
      vector2d_t spot = {.x = current_tile->x, .y = current_tile->y - 1};
      if (!vector_contains(&vec, &spot)) {
        vector_append(&vec, &spot);
      }
    }

    current = current->next;
  }

  return vec;
}

bool game_is_space_available(game_t *game, int x, int y) {
  return *game_tile_at(game, x, y) == NULL;
}
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libcarcassonne/ext_base_game.h"
#include "libcarcassonne/forward.h"

return_code_t placed_tile_create(placed_tile_t     *placed_tile,
                                 const tile_t      *parent,
                                 tile_orientation_t orientation, int x, int y) {
  if (placed_tile == NULL || parent == NULL) return ERROR;

  placed_tile->parent      = parent;
  placed_tile->orientation = orientation;

  placed_tile->x = x;
  placed_tile->y = y;

  unsigned int max = 0;

  for (int i = 0; i < PLACED_TILE_GROUP_NUMBER; i++) {
    placed_tile_group_t **group =
        &placed_tile->groups[placed_tile->parent->parts_groups[i]];
    if (*group == NULL) {
      *group = calloc(1, sizeof(placed_tile_group_t));
      vector_alloc(&(*group)->neighbors, 8);
      (*group)->type = parent->parts[i];
    }

    if (placed_tile->parent->parts_groups[i] > max) {
      max = placed_tile->parent->parts_groups[i];
    }

    (*group)->tile = placed_tile;

    // si c'est pas un coin ou le centre
    if (i != 0 && i != 2 && i != 4 && i != 6 && i != 8) {
      (*group)->open_slots += 1;
    }
  }

  for (unsigned int i = 0; i <= max; i++) {
    printf(
        "Famille: %s,Groupe: %d, Open slots: %d, blason: %d\n",
        placed_tile->parent->family, i,
        (placed_tile->groups[placed_tile->parent->parts_groups[i]])->open_slots,
        (int)placed_tile->parent->blason);
  }

  return SUCCESS;
}

void placed_tile_destroy(placed_tile_t *placed_tile) {
  bool freed[9];
  memset(&freed, 0, sizeof(freed));

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!freed[group]) {
      vector_free(&placed_tile->groups[group]->neighbors);
      free(placed_tile->groups[group]);
    }
    freed[group] = true;
  }
}

static bool placed_tile_group_connected_inner(placed_tile_group_t *a,
                                              placed_tile_group_t *b,
                                              int                  marker) {
  // boucle
  if (a->marker == marker) {
    return false;
  }

  if (a == b) return true;

  // on le marque pour éviter de le re-visiter
  a->marker = marker;

  for (unsigned int i = 0; i < vector_size(&a->neighbors); i++) {
    if (placed_tile_group_connected_inner(*vector_nth(&a->neighbors, i), b,
                                          marker))
      return true;
  }

  return false;
}

static int dfs_counter = 1;

bool placed_tile_group_connected(placed_tile_group_t *a,
                                 placed_tile_group_t *b) {
  // pour essayer de parcourir
  int search = dfs_counter++;

  return placed_tile_group_connected_inner(a, b, search);
}

void placed_tile_group_cut(placed_tile_group_t *a, placed_tile_group_t *b) {
  // On coupe le lien dans le graphe
  vector_remove_value(&a->neighbors, &b);
  vector_remove_value(&b->neighbors, &a);

  a->open_slots++;
  b->open_slots++;
}
bool placed_tile_group_link(placed_tile_group_t *a, placed_tile_group_t *b) {
  // On les lie dans le graphe
  if (vector_contains(&a->neighbors, &b) ||
      vector_contains(&b->neighbors, &a)) {
    return false;
  }
  vector_append(&a->neighbors, &b);
  vector_append(&b->neighbors, &a);
  return true;
}

static int placed_tile_group_complete_inner(placed_tile_group_t *a,
                                            int                  marker) {
  // boucle
  if (a->marker == marker) {
    return 0;
  }

  int total = a->open_slots;
  // on le marque pour éviter de le re-visiter
  a->marker = marker;

  for (unsigned int i = 0; i < vector_size(&a->neighbors); i++) {
    total +=
        placed_tile_group_complete_inner(*vector_nth(&a->neighbors, i), marker);
  }

  return total;
}

bool placed_tile_group_complete(placed_tile_group_t *group) {
  int search_marker = dfs_counter++;

  return placed_tile_group_complete_inner(group, search_marker) == 0;
}

void placed_tile_group_create(placed_tile_group_t **group, meeple_t *meeple,
                              const placed_tile_t *tile) {
  (*group) = calloc(1, sizeof(placed_tile_group_t));
  vector_alloc(&(*group)->neighbors, 0);
  (*group)->meeple = meeple;
  (*group)->tile   = tile;
}

void placed_tile_group_destory(placed_tile_group_t *group) {
  vector_free(&group->neighbors);
  free(group);
}

static void placed_tile_group_collect_meeples_inner(
    placed_tile_group_t *group, placed_tile_group_eval_points_t *points,
    int marker, bool is_completed) {
  if (group->marker == marker) {
    return;
  }

  // on le marque pour éviter de le re-visiter
  group->marker = marker;

  unsigned int value = 0;
  if(group->type==LIBCARCASSONNE_TILE_PART_ROAD){
    value = 1;
  }
  else if (group->type==LIBCARCASSONNE_TILE_PART_TOWN) {
    value = 1;
    if (group->tile->parent->blason) {
      value++;
    }

    if(is_completed){
      value*=2;
    }
  }

  points->points+=value;

  if (group->meeple != NULL) {
    vector_append(&points->meeples, &group->meeple);
  }

  for (unsigned int i = 0; i < vector_size(&group->neighbors); i++) {
    placed_tile_group_collect_meeples_inner(*vector_nth(&group->neighbors, i),
                                            points, marker, is_completed);
  }
}

/**
 * @brief Renvoie la liste des meeples dans un groupe
 *
 * @param group Un noeud du groupe
 */
placed_tile_group_eval_points_t placed_tile_group_eval_points(
    placed_tile_group_t *group, bool is_completed) {
  int                             search_marker = dfs_counter++;
  placed_tile_group_eval_points_t points        = {0};

  placed_tile_group_collect_meeples_inner(group, &points, search_marker, is_completed);

  return points;
}

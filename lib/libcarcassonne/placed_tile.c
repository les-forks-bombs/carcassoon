#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

return_code_t placed_tile_get_face(placed_face_groups_t *ret,
                                   const placed_tile_t  *tile,
                                   tile_orientation_t    connexion_face) {
  if (tile == NULL) return ERROR;

  int index = (connexion_face + tile->orientation) % 4;

  static const int values[4][3] = {{0, 1, 2}, {2, 5, 8}, {8, 7, 6}, {6, 3, 0}};

  tile_part_group_t groups[3] = {tile->parent->parts_groups[values[index][0]],
                                 tile->parent->parts_groups[values[index][1]],
                                 tile->parent->parts_groups[values[index][2]]};

  ret->face[0] = tile->groups[groups[0]];
  ret->face[1] = tile->groups[groups[1]];
  ret->face[2] = tile->groups[groups[2]];

  return SUCCESS;
}

return_code_t placed_tile_create(placed_tile_t     *placed_tile,
                                 const tile_t      *parent,
                                 tile_orientation_t orientation) {
  if (placed_tile == NULL || parent == NULL) return ERROR;

  placed_tile->parent      = parent;
  placed_tile->orientation = orientation;
  for (int i = 0; i < 9; i++) {
    placed_tile_group_t **group =
        &placed_tile->groups[placed_tile->parent->parts_groups[i]];
    if (*group == NULL) {
      *group               = calloc(1, sizeof(placed_tile_group_t));
      (*group)->tile       = placed_tile;
      (*group)->open_slots = 0;  // todo: compter le nombre de fois ou le sous
                                 // groupe est sur les bords
      vector_alloc(&(*group)->neighbors, 1);
    }
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
void placed_tile_group_link(placed_tile_group_t *a, placed_tile_group_t *b) {
  // On les lie dans le graphe
  vector_append(&a->neighbors, &b);
  vector_append(&b->neighbors, &a);

  a->open_slots--;
  b->open_slots--;
}

static int placed_tile_group_complete_inner(placed_tile_group_t *a,
                                            int                  marker) {
  // boucle
  if (a->marker == marker) {
    return false;
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

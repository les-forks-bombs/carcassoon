#include <libutils/hashmap.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "libutils/lc.h"
#include "libutils/vector.h"

void utils_hashmap_alloc(hashmap_t *map, size_t number_of_buckets) {
  if (map == NULL || number_of_buckets < 1) {
    return ;
  }

  vector_alloc(&map->buckets, number_of_buckets);
  for (size_t i = 0; i < number_of_buckets; i++) {
    hashmap_node_list_t *list =
        ((hashmap_node_list_t *)vector_nth(&map->buckets, i));
    list->meta.head = NULL;
    list->meta.tail = NULL;
    list->meta.size = 0;
  }
  map->element_count = 0;
  map->size          = number_of_buckets;
}

void utils_hashmap_append(hashmap_t *map, char *key, char *data) {
  unsigned int    index    = hash(key);
  hashmap_node_t *new_node = calloc(1, sizeof(hashmap_node_t));
  new_node->key            = key;
  new_node->value          = data;

  hashmap_node_list_t *list =
      ((hashmap_node_list_t *)vector_nth(&map->buckets, index));

  list_prepend(list, &new_node);
}

char *utils_hashmap_get(const hashmap_t *map, char *key) {
  unsigned int index = hash(key);

  hashmap_node_list_t *list =
      ((hashmap_node_list_t *)vector_nth(&map->buckets, index));

  list_node_t    *list_node = list_head(list);
  hashmap_node_t *current   = ((hashmap_node_t *)list_value(list, list_node));
  while (list_node != NULL || strcmp(current->key, key) != 0) {
    list_node = list_node->next;
    current   = ((hashmap_node_t *)list_value(list, list_node));
  }
  return current->value;
}

void utils_hashmap_free(hashmap_t *map) {
  for (size_t i = 0; i < map->size; i++) {
    hashmap_node_list_t *list =
        ((hashmap_node_list_t *)vector_nth(&map->buckets, i));

    list_node_t *list_node = list_head(list);
    list_node_t *next;
    while (list_node != NULL) {
      next = list_node->next;
      free(((hashmap_node_t *)list_value(list, list_node)));
      list_node = next;
    }

    list_free(list);
  }
  vector_free(&map->buckets);
}

void utils_hashmap_remove(hashmap_t *map, char *key) {
  unsigned int index = hash(key);

  hashmap_node_list_t *list =
      ((hashmap_node_list_t *)vector_nth(&map->buckets, index));

  list_node_t    *list_node = list_head(list);
  hashmap_node_t *current   = ((hashmap_node_t *)list_value(list, list_node));
  while (list_node != NULL || strcmp(current->key, key) != 0) {
    list_node = list_node->next;
    current   = ((hashmap_node_t *)list_value(list, list_node));
  }

  list_remove(list, list_node);
}

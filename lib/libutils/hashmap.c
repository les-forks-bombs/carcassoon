#include "hashmap.h"

#include <libutils/hash.h>
#include <libutils/hashmap.h>
#include <libutils/lc.h>
#include <libutils/vector.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void utils_hashmap_create(hashmap_t *map, size_t number_of_buckets) {
  if (map == NULL || number_of_buckets < 1) {
    return;
  }
  list_t list;
  list.head = NULL;
  list.tail = NULL;
  list.size = 0;

  vector_alloc(&map->buckets, number_of_buckets);
  for (size_t i = 0; i < number_of_buckets; i++) {
    vector_append(&map->buckets, &list);
  }
  map->element_count = 0;
  map->size          = number_of_buckets;
}

void utils_hashmap_set(hashmap_t *map, const void *key, size_t key_size,
                       const void *data, size_t data_size) {
  unsigned int index = hash(key, key_size) % map->size;

  list_t *list = ((list_t *)vector_nth(&map->buckets, index));

  // Check for existing key to update it
  list_node_t *list_node = list->head;
  while (list_node != NULL) {
    hashmap_node_t *current = (hashmap_node_t *)list_node->value;
    if (current->key_size == key_size &&
        memcmp(current->key, key, key_size) == 0) {
      free(current->value);
      current->value = malloc(data_size);
      memcpy(current->value, data, data_size);
      return;
    }
    list_node = list_node->next;
  }

  // Key doesn't exist, create new node
  hashmap_node_t new_node;
  new_node.key = malloc(key_size);
  memcpy(new_node.key, key, key_size);
  new_node.value = malloc(data_size);
  memcpy(new_node.value, data, data_size);
  new_node.key_size = key_size;

  utils_list_prepend(list, &new_node, sizeof(hashmap_node_t));
  map->element_count++;
}

void *utils_hashmap_get(const hashmap_t *map, const void *key,
                        size_t key_size) {
  unsigned int index = hash(key, key_size) % map->size;

  list_t *list = ((list_t *)vector_nth(&map->buckets, index));

  if (list->head == NULL) {
    return NULL;
  }

  list_node_t *list_node = list->head;
  while (list_node != NULL) {
    hashmap_node_t *current = (hashmap_node_t *)list_node->value;
    if (current->key_size == key_size &&
        memcmp(current->key, key, key_size) == 0) {
      return current->value;
    }
    list_node = list_node->next;
  }

  return NULL;
}

void utils_hashmap_free(hashmap_t *map) {
  for (size_t i = 0; i < map->size; i++) {
    list_t *list = ((list_t *)vector_nth(&map->buckets, i));

    list_node_t *list_node = list->head;
    while (list_node != NULL) {
      hashmap_node_t *node = (hashmap_node_t *)list_node->value;
      free(node->key);
      free(node->value);
      list_node = list_node->next;
    }

    utils_list_free(list);
  }
  vector_free(&map->buckets);
}

void utils_hashmap_remove(hashmap_t *map, const void *key, size_t key_size) {
  unsigned int index = hash(key, key_size) % map->size;

  list_t *list = ((list_t *)vector_nth(&map->buckets, index));

  list_node_t *list_node = list->head;
  while (list_node != NULL) {
    hashmap_node_t *current = (hashmap_node_t *)list_node->value;
    if (current->key_size == key_size &&
        memcmp(current->key, key, key_size) == 0) {
      free(current->key);
      free(current->value);
      utils_list_remove(list, list_node);
      map->element_count--;
      return;
    }
    list_node = list_node->next;
  }
}

unsigned int hash(const void *key, size_t key_size) {
  return murmur3_32(key, key_size, LIBUTILS_HASHMAP_SEED);
}
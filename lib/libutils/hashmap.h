#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "libutils/lc.h"
#include "libutils/vector.h"

#define LIBUTILS_HASHMAP_SEED 67

typedef struct hashmap_node {
  char *key;
  char *value;
} hashmap_node_t;

typedef Vector(list_t) bucket_vector_t;

typedef struct hashmap {
  bucket_vector_t buckets;
  unsigned int    size;
  unsigned int    element_count;
} hashmap_t;

#define HashMap(key_type, value_type)           \
  union {                   \
    hashmap_t      meta;        \
    key_type   *key_ghost;   \
    value_type *value_ghost; \
  }

unsigned int hash(const void *key, size_t key_size);

void utils_hashmap_create(hashmap_t *map, size_t number_of_buckets);

void  utils_hashmap_set(hashmap_t *map, const void *key, size_t key_size, const void *data,
                        size_t data_size);
void *utils_hashmap_get(const hashmap_t *map, const void *key, size_t key_size);
void  utils_hashmap_free(hashmap_t *map);
void  utils_hashmap_remove(hashmap_t *map, void *key, size_t key_size);

#define hashmap_create(map, number_of_buckets) \
  utils_hashmap_create(&((map)->meta), number_of_buckets)

#define hashmap_set(map, key, value)                                 \
  utils_hashmap_set(&((map)->meta),                                  \
                    (const void *)(1 ? (key) : (map)->key_ghost),    \
                    sizeof(*(map)->key_ghost),                       \
                    (const void *)(1 ? (value) : (map)->value_ghost), \
                    sizeof(*(map)->value_ghost))

#define hashmap_get(map, key)                                       \
  ((__typeof__((map)->value_ghost))utils_hashmap_get(              \
      &((map)->meta), (const void *)(1 ? (key) : (map)->key_ghost), \
      sizeof(*(map)->key_ghost)))

#define hashmap_remove(map, key)                                     \
  utils_hashmap_remove(&((map)->meta),                               \
                       (const void *)(1 ? (key) : (map)->key_ghost), \
                       sizeof(*(map)->key_ghost))

#define hashmap_free(map) utils_hashmap_free(&((map)->meta))

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "libutils/vector.h"
#include "libutils/lc.h"

typedef struct hashmap_node {
  char* key;
  char* value;
} hashmap_node_t;

typedef List(hashmap_node_t*) hashmap_node_list_t;
typedef Vector(hashmap_node_list_t) bucket_vector_t;

typedef struct hashmap {
  bucket_vector_t   buckets;
  unsigned int size;
  unsigned int element_count;
} hashmap_t;

#define Map(K, V) \
    union { \
        map_t meta; \
        KeyType *key_ghost; \
        ValueType *value_ghost; \
    }

unsigned int hash(const char* key);

void utils_hashmap_alloc(hashmap_t *map, size_t number_of_buckets);

void  utils_hashmap_append(hashmap_t *map, char *key, char *data);
char *utils_hashmap_get(const hashmap_t *map, char* key);
void  utils_hashmap_free(hashmap_t *map);
void  utils_hashmap_remove(hashmap_t *map, char* key);

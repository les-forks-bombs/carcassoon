#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int size;
  unsigned int caps;
  void        *data;
} vector_t;

#define Vector(type)     \
  union {                \
    vector_t meta;       \
    type    *type_ghost; \
  }
unsigned int utils_vector_alloc(vector_t *list, size_t capacity,
                                size_t element);
unsigned int utils_vector_grow(vector_t *list, size_t size, size_t element);

unsigned int utils_vector_append(vector_t *list, const void *data, size_t size);
void *utils_vector_nth(const vector_t *list, unsigned int index, size_t size);
void  utils_vector_free(vector_t *list);
void  utils_vector_remove(vector_t *list, unsigned int index, size_t size);
void  utils_vector_remove_value(vector_t *list, const void *data, size_t size);

#define vector_append(list, item)                                      \
  utils_vector_append(&((list)->meta),                                 \
                      (const void *)(1 ? (item) : (list)->type_ghost), \
                      sizeof(*(list)->type_ghost))

#define vector_nth(list, index)                     \
  (__typeof__((list)->type_ghost))utils_vector_nth( \
      &((list)->meta), index, sizeof(*(list)->type_ghost))

#define vector_remove(list, item) \
  utils_vector_remove(&((list)->meta), item, sizeof(*(list)->type_ghost))

#define vector_free(list) utils_vector_free(&((list)->meta))
#define vector_alloc(list, size) \
  utils_vector_alloc(&((list)->meta), size, sizeof(*(list)->type_ghost))
#define vector_grow(list, size) \
  utils_vector_grow(&((list)->meta), size, sizeof(*(list)->type_ghost))

#define vector_size(list) (*(const unsigned int *)&((list)->meta.size))
#define vector_caps(list) (*(const unsigned int *)&((list)->meta.caps))

#define vector_remove_value(list, item)                                      \
  utils_vector_remove_value(&((list)->meta),                                 \
                            (const void *)(1 ? (item) : (list)->type_ghost), \
                            sizeof(*(list)->type_ghost))

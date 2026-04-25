#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned int size;
  unsigned int caps;
  void        *data;
} vector_t;

#define Vector(T)        \
  union {                \
    vector_t meta;       \
    T       *type_ghost; \
  }
int _vector_alloc(vector_t *list, size_t capacity, size_t element);
int _vector_grow(vector_t *list, size_t size, size_t element);

int   _vector_append(vector_t *list, const void *data, size_t size);
void *_vector_nth(const vector_t *list, unsigned int index, size_t size);
void  _vector_free(vector_t *list);
void  _vector_remove(vector_t *list, unsigned int index, size_t size);
void  _vector_remove_value(vector_t *list, const void *data, size_t size);

#define vector_append(list, item)                                    \
  _vector_append(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
                 sizeof(*(list)->type_ghost))

#define vector_nth(list, index)                                       \
  (__typeof__((list)->type_ghost))_vector_nth(&((list)->meta), index, \
                                              sizeof(*(list)->type_ghost))

#define vector_remove(list, item) \
  _vector_remove(&((list)->meta), item, sizeof(*(list)->type_ghost))

#define vector_free(list) _vector_free(&((list)->meta))
#define vector_alloc(list, size) \
  _vector_alloc(&((list)->meta), size, sizeof(*(list)->type_ghost))
#define vector_grow(list, size) \
  _vector_grow(&((list)->meta), size, sizeof(*(list)->type_ghost))

#define vector_size(list) (*(const unsigned int *)&((list)->meta.size))
#define vector_caps(list) (*(const unsigned int *)&((list)->meta.caps))

#define vector_remove_value(list, item)                                    \
  _vector_remove_value(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
                       sizeof(*(list)->type_ghost))

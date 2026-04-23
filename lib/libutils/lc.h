#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_node {
  struct list_node *next, *prev;
  char              value[];
} list_node_t;

typedef struct {
  list_node_t *head;
  list_node_t *tail;
  unsigned int size;
} list_t;

#define List(T)        \
  union {              \
    list_t meta;       \
    T     *type_ghost; \
  }

list_node_t *_list_append(list_t *list, const void *data, size_t size);
list_node_t *_list_prepend(list_t *list, const void *data, size_t size);
list_node_t *_list_insert(list_t *list, const void *data, size_t size,
                             unsigned int index);
list_node_t *_list_nth(list_t *list, unsigned int index);
void         _list_free(list_t *list);
void         _list_remove(list_t *list, list_node_t *data);
void         _list_remove_value(list_t *list, const void *data, size_t size);

#define list_append(list, item)                                    \
  _list_append(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
               sizeof(*(list)->type_ghost))

#define list_prepend(list, item)                                    \
  _list_prepend(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
                sizeof(*(list)->type_ghost))

#define list_insert(list, item, index)                             \
  _list_insert(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
                  sizeof(*(list)->type_ghost), index)

#define list_nth(list, index)   _list_nth(&((list)->meta), index)
#define list_value(list, node)    ((__typeof__((list)->type_ghost))((node)->value))
#define list_remove(list, item) _list_remove(&((list)->meta), item)

#define list_head(list) ((list)->meta.head)
#define list_tail(list) ((list)->meta.tail)

#define list_free(list) _list_free(&((list)->meta))

#define list_size(list) (*(const unsigned int *)&((list)->meta.size))

#define list_remove_value(list, item)                                    \
  _list_remove_value(&((list)->meta), (1 ? (item) : (list)->type_ghost), \
                   sizeof(*(list)->type_ghost))

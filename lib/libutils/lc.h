#pragma once

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

#define List(type)     \
  union {              \
    list_t meta;       \
    type  *type_ghost; \
  }

list_node_t *utils_list_append(list_t *list, const void *data, size_t size);
list_node_t *utils_list_prepend(list_t *list, const void *data, size_t size);
list_node_t *utils_list_insert(list_t *list, const void *data, size_t size,
                               unsigned int index);
list_node_t *utils_list_nth(list_t *list, unsigned int index);
void         utils_list_free(list_t *list);
void         utils_list_remove(list_t *list, list_node_t *data);
void utils_list_remove_value(list_t *list, const void *data, size_t size);

#define list_append(list, item)                                      \
  utils_list_append(&((list)->meta),                                 \
                    (const void *)(1 ? (item) : (list)->type_ghost), \
                    sizeof(*(list)->type_ghost))

#define list_prepend(list, item)                                      \
  utils_list_prepend(&((list)->meta),                                 \
                     (const void *)(1 ? (item) : (list)->type_ghost), \
                     sizeof(*(list)->type_ghost))

#define list_insert(list, item, index)                               \
  utils_list_insert(&((list)->meta),                                 \
                    (const void *)(1 ? (item) : (list)->type_ghost), \
                    sizeof(*(list)->type_ghost), index)

#define list_nth(list, index)  utils_list_nth(&((list)->meta), index)
#define list_value(list, node) ((__typeof__((list)->type_ghost))((node)->value))
#define list_remove(list, item) utils_list_remove(&((list)->meta), item)

#define list_head(list) ((list)->meta.head)
#define list_tail(list) ((list)->meta.tail)

#define list_free(list) utils_list_free(&((list)->meta))

#define list_size(list) (*(const unsigned int *)&((list)->meta.size))

#define list_remove_value(list, item)                                      \
  utils_list_remove_value(&((list)->meta),                                 \
                          (const void *)(1 ? (item) : (list)->type_ghost), \
                          sizeof(*(list)->type_ghost))

#include <libutils/vector.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _vector_alloc(vector_t *list, size_t capacity, size_t element) {
  if (list->caps >= capacity) return list->caps;

  list->caps = capacity;

  if (list->data == NULL) {
    list->data = malloc(list->caps * element);
  } else {
    list->data = realloc(list->data, list->caps * element);
  }

  return list->caps;
}

int _vector_grow(vector_t *list, size_t capacity, size_t element) {
  return _vector_alloc(list, capacity + list->caps, element);
}

int _vector_append(vector_t *list, const void *data, size_t size) {
  if (list->size + 1 > list->caps) _vector_grow(list, list->caps, size);

  memcpy(list->data + (size * list->size), data, size);
  list->size++;

  return list->size;
}

void *_vector_nth(vector_t *list, unsigned int index, size_t size) {
  if (index > list->size) return NULL;

  return list->data + (size * index);
}

void _vector_free(vector_t *list) {
  free(list->data);
  list->data = NULL;
  list->caps = 0;
  list->size = 0;
}

void _vector_remove(vector_t *list, unsigned int index, size_t size) {
  void *el   = _vector_nth(list, index, size);
  void *last = _vector_nth(list, list->size, size);

  memcpy(el, last, size);
  list->size--;
}

void _vector_remove_value(vector_t *list, const void *data, size_t size) {
  for (unsigned int i = 0; i < list->size; i++) {
    if (memcmp(data, _vector_nth(list, i, size), size) == 0) {
      _vector_remove(list, i, size);
      return;
    }
  }
}

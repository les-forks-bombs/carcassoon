#include <libutils/vector.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int _vector_alloc(vector_t *list, size_t capacity, size_t element) {
  if (list->caps >= capacity && list->data != NULL) return list->caps;

  list->caps = capacity <= 1 ? 1 : capacity;
  list->data = realloc(list->data, list->caps * element);

  return list->caps;
}

int _vector_grow(vector_t *list, size_t capacity, size_t element) {
  return _vector_alloc(list, capacity + list->caps, element);
}

int _vector_append(vector_t *list, const void *data, size_t size) {
  if (list->size + 1 > list->caps) _vector_grow(list, list->caps, size);

  memcpy((char *)list->data + (size * list->size), data, size);
  list->size++;

  return list->size;
}

void *_vector_nth(const vector_t *list, unsigned int index, size_t size) {
  if (index >= list->size) return NULL;

  return (char *)list->data + (size * index);
}

void _vector_free(vector_t *list) {
  if (list->data != NULL) {
    free(list->data);
  }
  list->data = NULL;
  list->caps = 0;
  list->size = 0;
}

void _vector_remove(vector_t *list, unsigned int index, size_t size) {
  if (index >= list->size) return;

  // on ignore le dernier car si on le supprime, il suffit juste de retirer un a
  // size
  if (index < list->size - 1) {
    void *el   = _vector_nth(list, index, size);
    void *last = _vector_nth(list, list->size - 1, size);
    memcpy(el, last, size);
  }
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

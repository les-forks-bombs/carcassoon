#include <libutils/vector.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

unsigned int utils_vector_alloc(vector_t *list, size_t capacity,
                                size_t element) {
  if (list->caps >= capacity && list->data != NULL) return list->caps;

  list->caps = capacity <= 1 ? 1 : capacity;
  list->data = realloc(list->data, list->caps * element);
  if (list->data == NULL) {
    printf("FATAL: Malloc Failure");
    exit(1);
  }

  return list->caps;
}

unsigned int utils_vector_grow(vector_t *list, size_t size, size_t element) {
  return utils_vector_alloc(list, size + list->caps, element);
}

unsigned int utils_vector_append(vector_t *list, const void *data,
                                 size_t size) {
  if (list->size + 1 > list->caps) {
    utils_vector_grow(list, list->caps, size);
  }

  memcpy((char *)list->data + (size * list->size), data, size);
  list->size++;

  return list->size;
}

void *utils_vector_nth(const vector_t *list, unsigned int index, size_t size) {
  if (index >= list->size) {
    return NULL;
  }

  return (char *)list->data + (size * index);
}

void utils_vector_free(vector_t *list) {
  if (list->data != NULL) {
    free(list->data);
  }
  list->data = NULL;
  list->caps = 0;
  list->size = 0;
}

void utils_vector_remove(vector_t *list, unsigned int index, size_t size) {
  if (index >= list->size) {
    return;
  }

  // on ignore le dernier car si on le supprime, il suffit juste de retirer un a
  // size
  if (index < list->size - 1) {
    void *element = utils_vector_nth(list, index, size);
    void *last    = utils_vector_nth(list, list->size - 1, size);
    memcpy(element, last, size);
  }
  list->size--;
}

void utils_vector_remove_value(vector_t *list, const void *data, size_t size) {
  for (unsigned int i = 0; i < list->size; i++) {
    if (memcmp(data, utils_vector_nth(list, i, size), size) == 0) {
      utils_vector_remove(list, i, size);
      return;
    }
  }
}

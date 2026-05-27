#include <libutils/vector.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

unsigned int utils_vector_alloc(vector_t *vector, size_t capacity,
                                size_t element) {
  if (vector->caps >= capacity && vector->data != NULL) return vector->caps;

  vector->caps = capacity <= 1 ? 1 : capacity;
  vector->data = realloc(vector->data, vector->caps * element);
  if (vector->data == NULL) {
    printf("FATAL: Malloc Failure");
    exit(1);
  }

  return vector->caps;
}

unsigned int utils_vector_grow(vector_t *vector, size_t size, size_t element) {
  return utils_vector_alloc(vector, size + vector->caps, element);
}

unsigned int utils_vector_append(vector_t *vector, const void *data,
                                 size_t size) {
  if (vector->size + 1 > vector->caps) {
    utils_vector_grow(vector, vector->caps, size);
  }

  memcpy((char *)vector->data + (size * vector->size), data, size);
  vector->size++;

  return vector->size;
}

void *utils_vector_nth(const vector_t *vector, unsigned int index,
                       size_t size) {
  if (index >= vector->size) {
    return NULL;
  }

  return (char *)vector->data + (size * index);
}

void utils_vector_free(vector_t *vector) {
  if (vector->data != NULL) {
    free(vector->data);
  }
  vector->data = NULL;
  vector->caps = 0;
  vector->size = 0;
}

void utils_vector_remove(vector_t *vector, unsigned int index, size_t size) {
  if (index >= vector->size) {
    return;
  }

  // on ignore le dernier car si on le supprime, il suffit juste de retirer un a
  // size
  if (index < vector->size - 1) {
    void *element = utils_vector_nth(vector, index, size);
    void *last    = utils_vector_nth(vector, vector->size - 1, size);
    memcpy(element, last, size);
  }
  vector->size--;
}

void utils_vector_remove_value(vector_t *vector, const void *data,
                               size_t size) {
  for (unsigned int i = 0; i < vector->size; i++) {
    if (memcmp(data, utils_vector_nth(vector, i, size), size) == 0) {
      utils_vector_remove(vector, i, size);
      return;
    }
  }
}

bool utils_vector_contains(vector_t *vector, const void *data, size_t size) {
  for (unsigned int i = 0; i < vector->size; i++) {
    if (memcmp(data, utils_vector_nth(vector, i, size), size) == 0) {
      return true;
    }
  }

  return false;
}
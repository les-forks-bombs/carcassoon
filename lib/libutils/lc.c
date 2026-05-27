#include <libutils/lc.h>
#include <stdlib.h>
#include <string.h>

static list_node_t *utils_list_create_node(const void *data, size_t size) {
  list_node_t *node = malloc(sizeof(list_node_t) + size);
  if (!node) {
    return NULL;
  }

  memcpy(node->value, data, size);
  node->next = node->prev = NULL;
  return node;
}

list_node_t *utils_list_append(list_t *list, const void *data, size_t size) {
  list_node_t *node = utils_list_create_node(data, size);
  if (!node) {
    return NULL;
  }

  if (list->tail) {
    list->tail->next = node;
    node->prev       = list->tail;
  }
  list->tail = node;
  if (!list->head) {
    list->head = node;
  }

  list->size++;
  return node;
}

list_node_t *utils_list_prepend(list_t *list, const void *data, size_t size) {
  list_node_t *element = utils_list_create_node(data, size);
  if (!element) {
    return NULL;
  }

  if (list->head) {
    list->head->prev = element;
    element->next    = list->head;
  }
  list->head = element;
  if (!list->tail) {
    list->tail = element;
  }

  list->size++;
  return element;
}

list_node_t *utils_list_insert(list_t *list, const void *data, size_t size,
                               unsigned int index) {
  if (index == 0) {
    return utils_list_prepend(list, data, size);
  }
  if (index >= list->size) {
    return utils_list_append(list, data, size);
  }

  list_node_t *successor = utils_list_nth(list, index);
  if (!successor) {
    return NULL;
  }

  list_node_t *element = utils_list_create_node(data, size);
  if (!element) {
    return NULL;
  }

  element->next = successor;
  element->prev = successor->prev;

  if (successor->prev) {
    successor->prev->next = element;
  }
  successor->prev = element;

  list->size++;
  return element;
}

void utils_list_remove(list_t *l, list_node_t *el) {
  if (l->head == el) {
    l->head = el->next;
  }

  if (l->tail == el) {
    l->tail = el->prev;
  }

  if (el->next != NULL) {
    el->next->prev = el->prev;
  }
  if (el->prev != NULL) {
    el->prev->next = el->next;
  }

  l->size--;

  free(el);
}

void utils_list_remove_value(list_t *l, const void *data, size_t size) {
  if (!l || !l->head) {
    return;
  }

  list_node_t *curr = l->head;
  while (curr) {
    if (memcmp(curr->value, data, size) == 0) {
      utils_list_remove(l, curr);
      return;
    }
    curr = curr->next;
  }
}

list_node_t *utils_list_nth(list_t *l, unsigned int index) {
  if (index < 0 || index > l->size) {
    return NULL;
  }

  list_node_t *current = l->head;
  for (unsigned int i = 0; i != index; i++) {
    current = current->next;
  }

  return current;
}

void utils_list_free(list_t *l) {
  while (l->size != 0) {
    utils_list_remove(l, l->head);
  }
}

#include <libutils/lc.h>
#include <string.h>

static list_node_t *_create_node(const void *data, size_t size) {
  list_node_t *node = malloc(sizeof(list_node_t) + size);
  if (!node) return NULL;

  memcpy(node->value, data, size);
  node->next = node->prev = NULL;
  return node;
}

list_node_t *_list_append(list_t *l, const void *data, size_t size) {
  list_node_t *el = _create_node(data, size);
  if (!el) return NULL;

  if (l->tail) {
    l->tail->next = el;
    el->prev      = l->tail;
  }
  l->tail = el;
  if (!l->head) l->head = el;

  l->size++;
  return el;
}

list_node_t *_list_prepend(list_t *l, const void *data, size_t size) {
  list_node_t *el = _create_node(data, size);
  if (!el) return NULL;

  if (l->head) {
    l->head->prev = el;
    el->next      = l->head;
  }
  l->head = el;
  if (!l->tail) l->tail = el;

  l->size++;
  return el;
}

list_node_t *_list_insert(list_t *l, const void *data, size_t size,
                             unsigned int index) {
  if (index == 0) return _list_prepend(l, data, size);
  if (index >= l->size) return _list_append(l, data, size);

  list_node_t *successor = _list_nth(l, index);
  if (!successor) return NULL;

  list_node_t *el = _create_node(data, size);
  if (!el) return NULL;

  el->next = successor;
  el->prev = successor->prev;

  if (successor->prev) successor->prev->next = el;
  successor->prev = el;

  l->size++;
  return el;
}

void _list_remove(list_t *l, list_node_t *el) {
  if (l->head == el) l->head = el->next;

  if (l->tail == el) l->tail = el->prev;

  if (el->next != NULL) el->next->prev = el->prev;
  if (el->prev != NULL) el->prev->next = el->next;

  l->size--;

  free(el);
}

void _list_remove_value(list_t *l, const void *data, size_t size) {
  if (!l || !l->head) return;

  list_node_t *curr = l->head;
  while (curr) {
    if (memcmp(curr->value, data, size) == 0) {
      _list_remove(l, curr);
      return;
    }
    curr = curr->next;
  }
}

list_node_t *_list_nth(list_t *l, unsigned int index) {
  if (index < 0 || index > l->size) return NULL;

  list_node_t *current = l->head;
  for (unsigned int i = 0; i != index; i++) current = current->next;

  return current;
}

void _list_free(list_t *l) {
  while (l->size != 0) _list_remove(l, l->head);
}

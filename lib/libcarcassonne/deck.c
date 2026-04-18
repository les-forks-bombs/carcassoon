#include <libcarcassonne/deck.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/prng_mersenne_twister.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

deck_t create_deck(int seed, extension_list_t* extensions) {
  // On initialise la struct, cette dernière sera stoquée dans le stack
  deck_t deck = {.list =
                     {
                         .head = NULL,
                         .tail = NULL,
                         .size = 0,
                     },
                 .state = create_prng_mersenne_twister_state(seed)};

  // On crée une queue (linked-list) qui sera utilisée pour
  // traiter les éléments un par un.
  deck_list_t queue = {
      .head = NULL,
      .tail = NULL,
      .size = 0,
  };

  unsigned int             start_tile_priority = 0;
  const extension_tiles_t* start_tile          = NULL;

  for (unsigned int j = 0; j < extensions->size; j++) {
    // A partir d'une composition de deck,
    // on ajoute les éléments a la queue
    for (unsigned int i = 0; i < extensions->extensions[j].tiles->size; i++) {
      const tile_t* element = &extensions->extensions[j].tiles->tiles[i];

      for (unsigned int j = 0; j < element->amount; j++) {
        deck_list_append(&queue, element);
      }
    }

    // On cherche la liste de tiles de démarrage avec la plus haute priorité
    if ((extensions->extensions[j].start_tiles_priority >
         start_tile_priority) ||
        start_tile == NULL) {
      start_tile_priority = extensions->extensions[j].start_tiles_priority;
      start_tile          = extensions->extensions[j].start_tiles;
    }
  }

  while (queue.size != 0) {
    int index = prng_mersenne_twister_random(&deck.state) % queue.size;

    // Récupérer l'élément a l'index n (O(n))
    deck_element_t* element = deck_list_nth(&queue, index);

    // on ajoute l'élément au deck
    deck_list_append(&deck.list, element->tile);

    // on le retire aussi de la queue
    deck_list_remove(&queue, element);
  }

  int index = prng_mersenne_twister_random(&deck.state) % start_tile->size;
  deck_list_prepend(&deck.list, &start_tile->tiles[index]);

  return deck;
}

void free_deck(deck_t deck) { deck_list_free(&deck.list); }

const tile_t* deck_pick(deck_t* deck) {
  if (deck->list.head == NULL) return NULL;

  const tile_t* el = deck->list.head->tile;
  deck_list_remove(&deck->list, deck->list.head);

  return el;
}

void deck_defausser(deck_t* deck, const tile_t* tile) {
  int index = prng_mersenne_twister_random(&deck->state) % deck->list.size;
  deck_list_insert(&deck->list, index, tile);
}

deck_list_t* deck_list_append(deck_list_t* l, const tile_t* tile) {
  deck_element_t* el = malloc(sizeof(deck_element_t));
  el->next = el->prev = NULL;
  el->tile            = tile;

  if (l == NULL) {
    // the list doesn't exist, we create it
    deck_list_t* t = malloc(sizeof(deck_list_t));
    t->head        = el;
    t->tail        = el;
    t->size        = 1;

    return t;
  }

  if (l->tail != NULL) l->tail->next = el;
  el->prev = l->tail;

  l->tail = el;
  if (l->head == NULL) l->head = el;
  l->size++;

  return l;
}

deck_list_t* deck_list_prepend(deck_list_t* l, const tile_t* tile) {
  deck_element_t* el = malloc(sizeof(deck_element_t));
  el->next = el->prev = NULL;
  el->tile            = tile;

  if (l == NULL) {
    // the list doesn't exist, we create it
    deck_list_t* t = malloc(sizeof(deck_list_t));
    t->head        = el;
    t->tail        = el;
    t->size        = 1;

    return t;
  }

  if (l->head != NULL) l->head->prev = el;
  el->next = l->head;

  l->head = el;
  l->size++;

  return l;
}

deck_list_t* deck_list_insert(deck_list_t* l, unsigned int index,
                              const tile_t* tile) {
  if (l == NULL) {
    return NULL;
  }
  if (index > l->size || index < 0) {
    return NULL;
  }

  deck_element_t* current = deck_list_nth(l, index);
  if (current == NULL) {
    return NULL;
  }

  deck_element_t* el = malloc(sizeof(deck_element_t));
  el->tile           = tile;

  el->prev = current;
  el->next = current->next;

  current->next       = el;
  current->next->prev = el;

  l->size++;

  return l;
}

void deck_list_remove(deck_list_t* l, deck_element_t* el) {
  if (l->head == el) l->head = el->next;

  if (l->tail == el) l->tail = el->prev;

  if (el->next != NULL) el->next->prev = el->prev;
  if (el->prev != NULL) el->prev->next = el->next;

  l->size--;

  free(el);
}

deck_element_t* deck_list_nth(deck_list_t* l, unsigned int index) {
  if (index < 0 || index > l->size) return NULL;

  deck_element_t* current = l->head;
  for (unsigned int i = 0; i != index; i++) current = current->next;

  return current;
}

void deck_list_free(deck_list_t* l) {
  while (l->size != 0) deck_list_remove(l, l->head);
}

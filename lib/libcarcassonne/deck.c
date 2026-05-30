#include <assert.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libutils/lc.h>
#include <libutils/vector.h>
#include <unistd.h>

deck_t create_deck(int seed, extension_vector_t* extensions) {
  // On initialise la struct, cette dernière sera stoquée dans le stack
  deck_t deck = {.list  = {.meta =
                               {
                                   .head = NULL,
                                   .tail = NULL,
                                   .size = 0,
                              }},
                 .state = create_prng_mersenne_twister_state(seed)};

  // On crée une queue (linked-list) qui sera utilisée pour
  // traiter les éléments un par un.
  deck_list_t queue = {.meta = {
                           .head = NULL,
                           .tail = NULL,
                           .size = 0,
                       }};

  unsigned int         start_tile_priority = 0;
  const tile_vector_t* start_tile          = NULL;

  for (unsigned int i = 0; i < vector_size(extensions); i++) {
    // A partir d'une composition de deck,
    // on ajoute les éléments a la queue
    const extension_t* extension = *(vector_nth(extensions, i));
    for (unsigned int j = 0; j < vector_size(extension->tiles); j++) {
      const tile_t* element = vector_nth(extension->tiles, j);
      for (unsigned int k = 0; k < element->amount; k++) {
        list_append(&queue, &element);
      }
    }

    // On cherche la liste de tiles de démarrage avec la plus haute priorité
    if ((extension->start_tiles_priority > start_tile_priority) ||
        start_tile == NULL) {
      start_tile_priority = extension->start_tiles_priority;
      start_tile          = extension->start_tiles;
    }
  }

  assert(start_tile != NULL);

  deck_list_t start_tiles = {.meta = {
                                 .head = NULL,
                                 .tail = NULL,
                                 .size = 0,
                             }};

  for (unsigned int i = 0; i < vector_size(start_tile); i++) {
    const tile_t* element = vector_nth(start_tile, i);

    for (unsigned int j = 0; j < element->amount; j++) {
      list_append(&start_tiles, &element);
    }
  }

  while (list_size(&queue) != 0) {
    unsigned int index =
        prng_mersenne_twister_random(&deck.state) % list_size(&queue);

    // Récupérer l'élément a l'index n (O(n))
    list_node_t* element = list_nth(&queue, index);

    // on ajoute l'élément au deck
    list_append(&deck.list, list_value(&queue, element));

    // on le retire aussi de la queue
    list_remove(&queue, element);
  }

  while (list_size(&start_tiles) != 0) {
    unsigned int index =
        prng_mersenne_twister_random(&deck.state) % list_size(&start_tiles);

    // Récupérer l'élément a l'index n (O(n))
    list_node_t* element = list_nth(&start_tiles, index);

    // on ajoute l'élément au deck
    list_prepend(&deck.list, list_value(&start_tiles, element));

    // on le retire aussi de la queue
    list_remove(&start_tiles, element);
  }

  return deck;
}

void free_deck(deck_t deck) { list_free(&deck.list); }

const tile_t* deck_pick(deck_t* deck) {
  if (list_size(&deck->list) == 0) {
    return NULL;
  }

  list_node_t*  el   = list_head(&deck->list);
  const tile_t* tile = *list_value(&deck->list, el);

  list_remove(&deck->list, el);

  return tile;
}

unsigned int deck_defausser(deck_t* deck, const tile_t* tile) {
  unsigned int index =
      prng_mersenne_twister_random(&deck->state) % list_size(&deck->list);
  list_insert(&deck->list, &tile, index);

  return index;
}

void deck_push(deck_t* deck, const tile_t* tile) {
  list_prepend(&deck->list, &tile);
}

const tile_t* deck_find_tile(deck_t* deck, const char* family, bool blason) {
  list_node_t* curr = list_head(&deck->list);

  while (curr != NULL &&
         (strcmp((*list_value(&deck->list, curr))->family, family) != 0 ||
          (*list_value(&deck->list, curr))->blason != blason)) {
    curr = curr->next;
  }

  if (curr == NULL) {
    return NULL;
  }

  const tile_t* tile = *list_value(&deck->list, curr);
  list_remove(&deck->list, curr);

  return tile;
}

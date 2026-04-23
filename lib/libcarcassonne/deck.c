#include <libcarcassonne/deck.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/prng_mersenne_twister.h>
#include <libcarcassonne/tile.h>
#include <libutils/lc.h>
#include <unistd.h>

deck_t create_deck(int seed, extension_list_t* extensions) {
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

  unsigned int             start_tile_priority = 0;
  const extension_tiles_t* start_tile          = NULL;

  for (unsigned int j = 0; j < extensions->size; j++) {
    // A partir d'une composition de deck,
    // on ajoute les éléments a la queue
    for (unsigned int i = 0; i < extensions->extensions[j].tiles->size; i++) {
      const tile_t* element = &extensions->extensions[j].tiles->tiles[i];

      for (unsigned int j = 0; j < element->amount; j++) {
        list_append(&queue, &element);
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

  while (list_size(&queue) != 0) {
    int index = prng_mersenne_twister_random(&deck.state) % list_size(&queue);

    // Récupérer l'élément a l'index n (O(n))
    list_node_t* element = list_nth(&queue, index);

    // on ajoute l'élément au deck
    list_append(&deck.list, list_value(&queue, element));

    // on le retire aussi de la queue
    list_remove(&queue, element);
  }

  int index = prng_mersenne_twister_random(&deck.state) % start_tile->size;
  list_prepend(&deck.list, &(start_tile->tiles) + index);

  return deck;
}

void free_deck(deck_t deck) { list_free(&deck.list); }

const tile_t* deck_pick(deck_t* deck) {
  if (list_size(&deck->list) == 0) return NULL;

  list_node_t*  el   = list_head(&deck->list);
  const tile_t* tile = *list_value(&deck->list, el);

  list_remove(&deck->list, el);

  return tile;
}

void deck_defausser(deck_t* deck, const tile_t* tile) {
  int index =
      prng_mersenne_twister_random(&deck->state) % list_size(&deck->list);
  list_insert(&deck->list, &tile, index);
}

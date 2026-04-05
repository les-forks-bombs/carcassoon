#include <libcarcassonne/deck.h>
#include <libcarcassonne/deck_composition.h>
#include <stdio.h>
#include <stdlib.h>

deck_t create_deck(
    int seed)
{
    // On initialise la struct, cette dernière sera stoquée dans le stack
    deck_t deck = {
        .list = {
            .head = NULL,
            .tail = NULL,
            .size = 0,
        },
        .state = create_prng_mersenne_twister_state(seed)
    };

    // On crée une queue (linked-list) qui sera utilisée pour
    // traiter les éléments un par un.
    deck_list_t queue = {
        .head = NULL,
        .tail = NULL,
        .size = 0,
    };

    // A partir d'une composition de deck,
    // on ajoute les éléments a la queue
    for (
        unsigned int i = 0;
        i < sizeof(tiles) / sizeof(tiles[0]);
        i++)
    {
        deck_tile_prototype_t *element = &tiles[i];

        for (
            int j = 0;
            j < element->remaining;
            j++)
        {
            deck_list_append(&queue, &element->tile);
        }
    }

    while (queue.size != 0)
    {
        int index = prng_mersenne_twister_random(&deck.state) % queue.size;

        // Récupérer l'élément a l'index n (O(n))
        deck_element_t *element = deck_list_nth(&queue, index);

        // on ajoute l'élément au deck
        deck_list_append(&deck.list, element->tile);

        // on le retire aussi de la queue
        deck_list_remove(&queue, element);
    }

    deck_list_prepend(&deck.list, &start_tile.tile);

    return deck;
}

void free_deck(
    deck_t deck)
{
    deck_list_free(&deck.list);
}

tile_t *deck_pick(deck_t *deck)
{
    if (deck->list.head == NULL)
        return NULL;

    tile_t *el = deck->list.head->tile;
    deck_list_remove(&deck->list, deck->list.head);

    return el;
}

void deck_defausser(deck_t *deck, tile_t *tile)
{
    int index = prng_mersenne_twister_random(&deck->state) % deck->list.size;
    deck_list_insert(&deck->list, index, tile);
}

deck_list_t *deck_list_append(
    deck_list_t *l,
    tile_t *tile)
{
    deck_element_t *el = malloc(sizeof(deck_element_t));
    el->next = el->prev = NULL;
    el->tile = tile;

    if (l == NULL)
    {
        // the list doesn't exist, we create it
        deck_list_t *t = malloc(sizeof(deck_list_t));
        t->head = el;
        t->tail = el;
        t->size = 1;

        return t;
    }

    if (l->tail != NULL)
        l->tail->next = el;
    el->prev = l->tail;

    l->tail = el;
    if (l->head == NULL)
        l->head = el;
    l->size++;

    return l;
}

deck_list_t *deck_list_prepend(
    deck_list_t *l,
    tile_t *tile)
{
    deck_element_t *el = malloc(sizeof(deck_element_t));
    el->next = el->prev = NULL;
    el->tile = tile;

    if (l == NULL)
    {
        // the list doesn't exist, we create it
        deck_list_t *t = malloc(sizeof(deck_list_t));
        t->head = el;
        t->tail = el;
        t->size = 1;

        return t;
    }

    if (l->head != NULL)
        l->head->prev = el;
    el->next = l->head;

    l->head = el;
    l->size++;

    return l;
}

deck_list_t *deck_list_insert(
    deck_list_t *l,
    unsigned int index,
    tile_t *tile)
{
    if (l == NULL)
    {
        return NULL;
    }
    if (index > l->size || index < 0)
    {
        return NULL;
    }

    deck_element_t *current = deck_list_nth(l, index);
    if (current == NULL)
    {
        return NULL;
    }

    deck_element_t *el = malloc(sizeof(deck_element_t));
    el->tile = tile;

    el->prev = current;
    el->next = current->next;

    current->next = el;
    current->next->prev = el;

    l->size++;

    return l;
}

void deck_list_remove(
    deck_list_t *l,
    deck_element_t *el)
{
    if (l->head == el)
        l->head = el->next;

    if (l->tail == el)
        l->tail = el->prev;

    if (el->next != NULL)
        el->next->prev = el->prev;
    if (el->prev != NULL)
        el->prev->next = el->next;

    l->size--;

    free(el);
}

deck_element_t *deck_list_nth(
    deck_list_t *l,
    unsigned int index)
{
    if (index < 0 || index > l->size)
        return NULL;

    deck_element_t *current = l->head;
    for (unsigned int i = 0; i != index; i++)
        current = current->next;

    return current;
}

void deck_list_free(
    deck_list_t *l)
{
    while (l->size != 0)
        deck_list_remove(l, l->head);
}

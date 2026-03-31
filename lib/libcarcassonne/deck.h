#ifndef H_LIBCARCASSONNE_DECK
#define H_LIBCARCASSONNE_DECK

#include <libcarcassonne/tile.h>
#include <libcarcassonne/consts.h>

typedef struct deck_tile
{
    tile_t tile;
    int remaining;
} deck_tile_t;

typedef struct deck
{
    int seed;
    tile_t tile[LIBCARCASSONNE_TILES_COUNT];
    int size;
} deck_t;

/// @brief Permet de créer une instance de deck
/// @param seed La seed qui sera utilisée pour la rng
/// @return 
deck_t  create_deck(int);

/// @brief Permet de piocher une tile dans le deck
/// @param deck Le deck qui sera utilisé pour piocher la tile
/// @return La tile piochée
tile_t* pick(deck_t*);

/// @brief Permet de remettre une tile dans le deck
/// @details Est utilisé dans le cas ou la tile proposée n'est pas utilisable
/// @param deck Le deck ciblé
/// @param tile La tile a remettre dans la pile 
void    empiler(deck_t*, tile_t*);

#endif

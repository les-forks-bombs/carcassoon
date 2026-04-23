#pragma once

#include <libcarcassonne/consts.h>
#include <libcarcassonne/prng_mersenne_twister.h>
#include <libcarcassonne/tile.h>
#include <libutils/lc.h>

struct extensions_list;

typedef List(const tile_t*) deck_list_t;

/// @brief Le deck permet de piocher et défausser des tiles
typedef struct deck {
  /// @brief La liste des tiles dans un ordre aléatoire
  deck_list_t list;
  /// @brief State du générateur de nombres pseudo-aléatoire
  prng_mersenne_twister_state_t state;
} deck_t;

/// @brief Permet de créer une instance de deck
/// @param seed La seed qui sera utilisée pour la rng
/// @param extensions The list of extensions to include in the deck
/// @return une instance de deck
/// @attention Le deck vis dans le stack!
/// @relates deck
deck_t create_deck(int seed, struct extensions_list* extensions);

/// @brief Permet de libérer la mémoire liée a un deck
/// @param deck Le deck a libérer
/// @relates deck
void free_deck(deck_t deck);

/// @brief Permet de piocher une tile dans le deck
/// @param deck Le deck qui sera utilisé pour piocher la tile
/// @return La tile piochée
/// @relates deck
const tile_t* deck_pick(deck_t* deck);

/// @brief Permet de remettre une tile dans le deck
/// @details Est utilisé dans le cas ou la tile proposée n'est pas utilisable
/// @param deck Le deck ciblé
/// @param tile La tile a remettre dans la pile
/// @relates deck
void deck_defausser(deck_t* deck, const tile_t* tile);

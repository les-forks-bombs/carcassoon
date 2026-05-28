#pragma once

#include <libcarcassonne/forward.h>
#include <libutils/lc.h>
#include <libutils/prng_mersenne_twister.h>

/// @brief Le deck permet de piocher et défausser des tiles
struct deck {
  /// @brief La liste des tiles dans un ordre aléatoire
  deck_list_t list;
  /// @brief State du générateur de nombres pseudo-aléatoire
  prng_mersenne_twister_state_t state;
};

/// @brief Permet de créer une instance de deck
/// @param seed La seed qui sera utilisée pour la rng
/// @param extensions La listes des extensions à inclure dans le deck
/// @return Une instance de deck
/// @attention Le deck vis dans le stack!
/// @relates deck_t
deck_t create_deck(int seed, extension_vector_t* extensions);

/// @brief Permet de libérer la mémoire liée a un deck
/// @param deck Le deck a libérer
/// @relates deck_t
void free_deck(deck_t deck);

/// @brief Permet de piocher une tile dans le deck
/// @param deck Le deck qui sera utilisé pour piocher la tile
/// @return La tile piochée
/// @relates deck_t
const tile_t* deck_pick(deck_t* deck);

/// @brief Permet de remettre une tile dans le deck
/// @details Est utilisé dans le cas où la tile proposée n'est pas utilisable
/// @param deck Le deck ciblé
/// @param tile La tile a remettre dans la pile
/// @return L'index auquel la tuile a été placé
/// @relates deck_t
int deck_defausser(deck_t* deck, const tile_t* tile);

/// @brief Permet de remettre une tile dans le deck au début
/// @details Est utilisé dans le cas où on annule une pose de tuile
/// @param deck Le deck ciblé
/// @param tile La tile a remettre dans la pile
/// @relates deck
void deck_push(deck_t* deck, const tile_t* tile);

/// @brief Permet de récupérer le prototype de tuile en spécifiant sa famille et
/// si elle a un blason
/// @details Est utilisé dans les tests pour créer des scénarios spécifique
/// @param deck Le deck ciblé
/// @param family La famille de la tuile recherché
/// @param blason true si la tuile possède un blason, false sinon
/// @return Un pointeur constant vers le prototype de la tuile
/// @attention La tuile retournée est retirée du deck
/// @relates deck_t
const tile_t* deck_find_tile(deck_t* deck, char* family, bool blason);

#ifndef H_LIBCARCASSONNE_DECK
#define H_LIBCARCASSONNE_DECK

#include <libcarcassonne/tile.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/deck_composition.h>

/// @brief Element dans la pile deck_list_t, double chainé (next, prev)
typedef struct deck_element
{
    /// @brief Référence vers la tile lié
    tile_t *tile;
    /// @brief Prochain élément dans la liste chainée
    struct deck_element *next;
    /// @brief Prédecesseur a l'élément courrant
    struct deck_element *prev;
} deck_element_t;

/// @brief Structure de liste chainée utilisée comme pile pour la pioche
/// @details Accès a la tête en O(1), ajout d'élément a tel index en O(n), insertion au début ou la fin en O(1)
typedef struct deck_list
{
    /// @brief Tête du tableau
    deck_element_t *head;
    /// @brief Queue du tableau
    deck_element_t *tail;
    /// @brief Nombre d'éléments dans la liste
    int size;
} deck_list_t;

/// @brief Le deck permet de piocher et défausser des tiles
typedef struct deck
{
    // todo: développer un prng qui gère son état dans une struct
    deck_list_t list;
} deck_t;

/// @brief Permet de créer une instance de deck
/// @param seed La seed qui sera utilisée pour la rng
/// @return une instance de deck
/// @attention Le deck vis dans le stack!
deck_t create_deck(int);

/// @brief Permet de libérer la mémoire liée a un deck
/// @param deck Le deck a libérer
void free_deck(deck_t);

/// @brief Permet de piocher une tile dans le deck
/// @param deck Le deck qui sera utilisé pour piocher la tile
/// @return La tile piochée
tile_t *pick(deck_t *);

/// @brief Permet de remettre une tile dans le deck
/// @details Est utilisé dans le cas ou la tile proposée n'est pas utilisable
/// @param deck Le deck ciblé
/// @param tile La tile a remettre dans la pile
void defausser(deck_t *, tile_t *);

/// @brief Ajoute un éléments a la queue de la liste
/// @param deck_list La liste chainée
/// @param tile La tile a ajouter
/// @returns La nouvelle liste chainée
deck_list_t *deck_list_append(deck_list_t *, tile_t *);

/// @brief Ajoute un éléments a la tête de la liste
/// @param deck_list La liste chainée
/// @param tile La tile a ajouter
/// @returns La nouvelle liste chainée
deck_list_t *deck_list_prepend(deck_list_t *, tile_t *);

/// @brief Insère in élément a un index donné
/// @param deck_list La liste chainée
/// @param index L'index ou insérer l'élément
/// @param tile La tile a insérer
/// @returns La nouvelle liste chainée
deck_list_t *deck_list_insert(deck_list_t *, int, tile_t *);

/// @brief Supprime un élément de la liste chainée
/// @param element L'élément a supprimer
void deck_list_remove(deck_list_t *, deck_element_t *);

/// @brief Récupère le n-ième élément de la liste
/// @param deck_list La liste chainée
/// @param index L'index de l'élément a récupérer
/// @return L'élément, peut être NULL si l'index est out-of-bounds
deck_element_t *deck_list_nth(deck_list_t *, int);

/// @brief Nettoie la mémoire d'une liste chainée
/// @param deck_list La liste chainée a nettoyer
void deck_list_free(deck_list_t *);

#endif

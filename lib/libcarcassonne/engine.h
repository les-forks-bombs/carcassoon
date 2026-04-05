#ifndef H_LIBCARCASSONNE_ENGINE
#define H_LIBCARCASSONNE_ENGINE

#include <libcarcassonne/game.h>
#include <libcarcassonne/deck.h>

/// @brief Représente un moteur pour une partie de carcassonne
typedef struct engine
{
    /// @brief La statut du plateau de jeu
    game_t game;
    /// @brief Le deck de cartes initialisé
    deck_t deck;
} engine_t;

#endif

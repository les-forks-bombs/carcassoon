#ifndef H_LIBCARCASSONNE_GAME
#define H_LIBCARCASSONNE_GAME

#include <libcarcassonne/consts.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>

/// @brief Représente une partie
typedef struct game
{
    /// @brief Index du joueur actuel
    unsigned int current_player;
    /// @brief Nombre total de joueurs dans la partie
    unsigned int players_count;
    /// @brief Tableau avec la liste des joueurs
    player_t players[LIBCARCASSONNE_MAX_PLAYERS];

    /// @brief Index du tour courrant
    unsigned int turn;
    /// @brief
    unsigned int turns_limit;

    /// @brief Instance du deck pour la partie
    deck_t deck;

    /// @brief Pointeur vers le premier élément de la map
    placed_tile_t **map;
} game_t;

/// @brief Initialise un objet `game` en mémoire
/// @param game L'emplacement ou créer le jeu
/// @param players_count Le nombre de joueurs
/// @param ai_count Le nombre d'IAs
/// @param seed La seed pour le deck
/// @param turns_limit La limite du nombre de tours
/// @return Status de la création du jeu
/// @related game_t
return_code_t create_game(
    game_t *game,
    unsigned int players_count,
    unsigned int ai_count,
    unsigned int seed,
    unsigned int turns_limit);

/// @brief Détruis & dé-alloue le jeu
/// @param game Le jeu a dé-allouer
/// @related game_t
void destroy_game(game_t *game);

/// @brief Retourne une référence vers l'emplacement ou la tile est enregistrée
/// @param x La cordonnée x
/// @param y La cordonnée y
/// @return Une référence vers l'emplacement dans `map`
/// @related game_t
placed_tile_t **game_tile_at(
    game_t *game,
    int x,
    int y);

void game_print_map(game_t *);
return_code_t game_place_tile(game_t *, tile_t *tile, int x, int y, tile_orientation_t orientation);

bool game_is_tile_placeable(game_t *game, tile_t *tile, int x, int y, tile_orientation_t orientation);

#endif

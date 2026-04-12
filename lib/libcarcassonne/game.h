#ifndef H_LIBCARCASSONNE_GAME
#define H_LIBCARCASSONNE_GAME

#include <libcarcassonne/consts.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>
#include <libcarcassonne/vector2d.h>

/// @brief Élement dans la liste chaîné list_tile_t
typedef struct tile_list_element {
  /// @brief La tuile placée
  placed_tile_t* tile;
  /// @brief Le prochain élément de la liste
  struct tile_list_element* next;
  /// @brief L'élément précédent de la liste
  struct tile_list_element* prev;
} tile_list_element_t;

/// @brief Structure de liste chainée utilisée pour conserver toutes les tuiles
/// avec au moins 1 côté disponible
typedef struct tile_list
{
    /// @brief Tête de la liste
    tile_list_element_t *head;
    /// @brief Queue de la liste
    tile_list_element_t *tail;
    /// @brief Nombre d'éléments de la liste
    unsigned int size;
} tile_list_t;

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
    /// @brief Nombre de tours maximum de la partie
    /// @details Si turns_limit = 0, la partie n'a pas de limite de tour.
    unsigned int turns_limit;

  /// @brief Instance du deck pour la partie
  deck_t deck;
  /// @brief Pointeur vers le premier élément de la map
  placed_tile_t** map;
  /// @brief Instance de la liste des tuiles
  tile_list_t open_tiles;

  // todo: implémenter une liste de meeples pour garder les noeuds en mémoire
} game_t;

/// @brief Initialise un objet `game` en mémoire
/// @param game L'emplacement ou créer le jeu
/// @param players_count Le nombre de joueurs
/// @param ai_count Le nombre d'IAs
/// @param seed La seed pour le deck
/// @param turns_limit La limite du nombre de tours
/// @return Status de la création du jeu
/// @related game_t
return_code_t create_game(game_t *game, unsigned int players_count,
                          unsigned int ai_count, unsigned int seed,
                          unsigned int turns_limit);

/// @brief Détruis & dé-alloue le jeu
/// @param game Le jeu à dé-allouer
/// @related game_t
void destroy_game(game_t *game);

/// @brief Retourne une référence vers l'emplacement ou la tile est enregistrée
/// @param x La cordonnée x
/// @param y La cordonnée y
/// @return Une référence vers l'emplacement dans `map`
/// @related game_t
placed_tile_t **game_tile_at(game_t *game, int x, int y);

void          game_print_map(game_t*);
void          game_print_detail(game_t*, int x, int y, int zoom);

return_code_t game_place_tile(game_t*, tile_t* tile, int x, int y,
                              tile_orientation_t orientation);

/// @brief Détermine si une tuile peut être placé à l'emplacement désigné
/// @param game Le jeu dans lequel placer la tuile
/// @param tile La tuile à placer
/// @param x La position en abscisse
/// @param y La position en ordonnée
/// @param orientation L'orientation de la tuile
/// @return Un booléen représentant la capacité à placer la tuile à cet endroit
/// @related game_t
bool game_is_tile_placeable(game_t *game, tile_t *tile, int x, int y,
                            tile_orientation_t orientation);

/// @brief Permet de libérer la mémoire liée à une liste de tuile
/// @param tl La liste de tuile
void destroy_tile_list(tile_list_t *tl);

/// @brief Vérifie si la case est ouverte ou non
/// @param game La partie dans laquelle vérifier
/// @param x La position en x
/// @param y La position en y
/// @return true si la case est ouverte, false sinon
bool game_is_place_open(game_t *game, int x, int y);

/// @brief Ajoute une tuile en tête de liste
/// @param tl La liste dans laquelle ajouter
/// @param tile La tuile à ajouter
void game_add_open_tile(tile_list_t *tl, placed_tile_t *tile);

/// @brief Retire une tuile de la liste
/// @param tl La liste depuis laquelle retirer
/// @param tile La tuile à retirer
void game_remove_open_tile(tile_list_t *tl, placed_tile_t *tile);

/// @brief Instancie une liste chaînée de tuile
/// @return Une liste chaînée de tile vide
tile_list_t create_open_tiles_list();
#endif

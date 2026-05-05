#pragma once

#include <libcarcassonne/consts.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/meeple.h>
#include <libcarcassonne/options.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/player.h>
#include <libcarcassonne/tile.h>
#include <libcarcassonne/vector2d.h>
#include <libutils/lc.h>
#include <libutils/vector.h>

#include "libcarcassonne/forward.h"

/// @brief Représente une partie
struct game {
  /// @brief Index du joueur actuel
  unsigned int current_player;
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
  placed_tile_t **map;

  /// @brief Instance de la liste des tuiles
  placed_tile_list_t open_tiles;

  /// @brief Paramètres du jeu
  options_t *options;
};

/// @brief Initialise un objet `game` en mémoire
/// @param game L'emplacement ou créer le jeu
/// @param options Les options de la partie
/// @return Status de la création du jeu
/// @related game_t
return_code_t create_game(game_t *game, options_t *options);

/// @brief Détruis & dé-alloue le jeu
/// @param game Le jeu à dé-allouer
/// @related game_t
void destroy_game(game_t *game);

/// @brief Retourne une référence vers l'emplacement ou la tile est enregistrée
/// @param coordinates_x La cordonnée x
/// @param coordinates_y La cordonnée y
/// @return Une référence vers l'emplacement dans `map`
/// @related game_t
placed_tile_t **game_tile_at(game_t *game, int coordinates_x,
                             int coordinates_y);

return_code_t game_place_tile(game_t *, const tile_t *tile, int x, int y,
                              tile_orientation_t orientation);
return_code_t game_remove_tile(game_t *, int x, int y);

/// @brief Détermine si une tuile peut être placé à l'emplacement désigné
/// @param game Le jeu dans lequel placer la tuile
/// @param tile La tuile à placer
/// @param x La position en abscisse
/// @param y La position en ordonnée
/// @param orientation L'orientation de la tuile
/// @return Un booléen représentant la capacité à placer la tuile à cet endroit
/// @related game_t
bool game_is_tile_placeable(game_t *game, const tile_t *tile, int x, int y,
                            tile_orientation_t orientation);

/// @brief Vérifie si la case est ouverte ou non
/// @param game La partie dans laquelle vérifier
/// @param x La position en x
/// @param y La position en y
/// @return true si la case est ouverte, false sinon
bool game_is_place_open(game_t *game, int x, int y);

/// @brief Permet de savoir si une partie est terminée ou non
/// @return Vrai si la partie est terminé, Faux sinon
bool is_game_finished(game_t *game);

/// @brief Termine le tour du joueur du courant
/// @return Le code de statut de l'opération, SUCCESS en cas de réussite
return_code_t game_end_player_turn(game_t *game);

/// @brief Termine le tour de table
/// @return Le code de statut de l'opération, SUCCESS en cas de réussite
return_code_t game_end_round(game_t *game);

/// @brief Place un meeple à l'emplacement spécifié
/// @param game La partie auquel ajouter le meeple
/// @param x Les coordonnées en abscisse
/// @param y Les coordonnées en ordonnées
/// @param part_group L'index de la partie de groupe
/// @param meeple_type Le type de meeple à placer
/// @param player Référence vers le joueur qui pose le meeple
/// @return Le code de statut de l'opération, SUCCESS en cas de réussite
return_code_t game_place_meeple(game_t *game, int x, int y, int part_group,
                                meeple_type_t meeple_type, player_t*player);

return_code_t game_remove_meeple(game_t *game, int x, int y, int part_group);

player_t * game_get_current_player(game_t *game);
#pragma once

#include <libcarcassonne/deck.h>
#include <libcarcassonne/player.h>
#include <libutils/lc.h>
#include <libutils/vector.h>
#include <stdbool.h>

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
  /// @brief État actuel de la partie
  game_state_t state;
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
/// @param colonne La cordonnée x
/// @param ligne La cordonnée y
/// @return Une référence vers l'emplacement dans `map`
/// @related game_t
placed_tile_t **game_tile_at(game_t *game, int colonne, int ligne);

/// @brief Créer et place une tuile en fonction des paramètres
/// @param game La partie dans laquelle placer la tuile
/// @param tile Le prototype de tuile à placer
/// @param x La cordonnée en x de la tuile
/// @param y La cordonnée en y de la tuile
/// @param orientation L'orientation de la tuile
/// @return Un code de retour, SUCCESS si réussite
/// @related game_t
return_code_t game_place_tile(game_t *game, const tile_t *tile, int x, int y,
                              tile_orientation_t orientation);

/// @brief Retire une tuile présente à la position désignée
/// @param game La partie dans laquelle placer la tuile
/// @param x La cordonnée en x de la tuile
/// @param y La cordonnée en y de la tuile
/// @attention Signale si l'emplacement est vide ou hors-jeu
/// @return Un code de retour, SUCCESS si réussite
/// @related game_t
return_code_t game_remove_tile(game_t *game, int x, int y);

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
/// @param game La partie à regarder
/// @return Vrai si la partie est terminé, Faux sinon
bool is_game_finished(game_t *game);

/// @brief Termine le tour du joueur du courant
/// @param game La partie utilisée
/// @return Le code de statut de l'opération, SUCCESS en cas de réussite
return_code_t game_end_player_turn(game_t *game);

/// @brief Termine le tour de table
/// @param game La partie utilisée
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
                                meeple_type_t meeple_type, player_t *player);

/// @brief Retire un meeple de l'emplacement désigné
/// @param game La partie dans laquelle retirer le meeple
/// @param x La cordonnée en x de la tuile
/// @param y La cordonnée en y de la tuile
/// @param part_group Le groupe auquel retirer un meeple
/// @return Un code de retour, SUCCESS si réussite
/// @related game_t
return_code_t game_remove_meeple(game_t *game, int x, int y, int part_group);

/// @brief Permet de récupérer une référence vers le joueur courant
/// @param game La partie utilisée
/// @return Une référence vers le joueur courant de la partie
/// @related game_t
player_t *game_get_current_player(game_t *game);

/// @brief Génère un vecteur contenant les positions candidats (à valider donc) pour le placement d'une tuile 
/// @param game La partie utilisée
/// @attention Pensez à libérer ce vecteur correctement !!
/// @return Un vecteur de position déjà alloué
/// @related game_t
vector2d_vector_t game_get_available_space(game_t *game);

/// @brief Vérifie si l'empacement désigné est disponible
/// @param game La partie utilisée
/// @return true si l'emplacement ne contient pas déjà une tuile, false sinon
/// @related game_t
bool game_is_space_available(game_t *game, int x, int y);
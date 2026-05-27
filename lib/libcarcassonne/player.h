#pragma once

#include <libcarcassonne/enums.h>
#include <libcarcassonne/forward.h>
#include <stdbool.h>

struct player {
  /// @brief Identifiant unique du joueur
  /// @details L'id correspond à la position du joueur dans la liste des joueurs
  unsigned int id;
  /// @brief Le type du joueur
  player_type_t player_type;
  /// @brief Le score actuel du joueur
  unsigned int score;
  /// @brief Le nombre de meeple dont le joueur dispose
  meeple_count_vector_t meeples_count;
  /// @brief Le vecteur des meeples appartenant aux joueurs actuellement sur le
  /// terrain
  meeple_vector_t meeples;
};

/// @brief Permet de créer un joueur
/// @param type Le type du joueur à créer
/// @param meeples_count Le nombre de meeple de chaque type dont le joueur
/// dipose
/// @details L'id correspond à la position du joueur dans la liste des joueurs
/// @param id L'identifiant unique du joueur
/// @return Le joueur créé
player_t create_player(player_type_t type, meeple_count_vector_t* meeples_count,
                       unsigned int id);

/// @brief Libère la mémoire d'une instance de joueur
/// @param player Le joueur à libérer
void free_player(player_t* player);

/// @brief Détermine si le joueur a encore des meeples à placer
/// @param player Le joueur à regarder
/// @return true si le joueur a encore des meeples, false sinon
bool player_has_meeple_to_place(player_t* player);
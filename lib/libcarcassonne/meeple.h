#pragma once

#include <libcarcassonne/libcarcassonne.h>

/// @brief Représente un meeple
struct meeple {
  /// @brief Le joueur possédant le meeple
  player_t* player;
  /// @brief Le groupe de tuile de jeu auquel appartient le meeple
  placed_tile_group_t* group_node;
  /// @brief Le type du meeple
  meeple_type_t meeple_type;
  /// @brief Le groupe de prototype de tuile sur lequel se trouve le meeple
  tile_part_group_t group;
};

/// @brief Représente un nombre de meeple d'un certain type
struct meeple_count {
  /// @brief Le type de meeple
  meeple_type_t meeple_type;
  /// @brief Le nombre de meeple
  unsigned int count;
};

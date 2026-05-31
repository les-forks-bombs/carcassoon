#pragma once

#include <libcarcassonne/libcarcassonne.h>
#include <libutils/vector.h>

/// @brief Nœud de l'arbre MCTS (Monte Carlo Tree Search)
struct mcts_node;
typedef struct mcts_node mcts_node_t;

/// @brief Vecteur de pointeurs vers des nœuds enfants
typedef Vector(mcts_node_t *) mcts_node_children_t;

/// @brief Nœud de l'arbre de recherche Monte Carlo
struct mcts_node {
  /// @brief Nombre de fois que ce nœud a été visité
  int visits;
  /// @brief Score cumulé obtenu lors des passages par ce nœud
  int score;
  /// @brief Action ayant mené à ce nœud depuis son parent
  action_t action;
  /// @brief Pointeur vers le nœud parent (NULL pour la racine)
  mcts_node_t *parent;
  /// @brief Enfants du nœud, correspondant aux actions possibles depuis cet
  /// état
  mcts_node_children_t children;
};

/// @brief Fait jouer l'IA pour le tour courant en utilisant l'algorithme MCTS
/// @param engine Le moteur de jeu représentant l'état courant de la partie
/// @param max_iterations Le nombre maximal d'itérations de simulation à
/// effectuer
void ai_play_turn(engine_t *engine, int max_iterations);

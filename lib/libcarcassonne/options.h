#pragma once

#include <libcarcassonne/enums.h>
#include <libcarcassonne/extension.h>
#include <unistd.h>

/// @brief Représente les options disponibles pour une partie
struct options {
  /// @brief Le mode de jeu (sdl ou cli)
  options_mode_t mode;
  /// @brief Le nombre total de joueurs
  unsigned int players;
  /// @brief La graine aléatoire de la partie
  unsigned int seed;
  /// @brief Le nombre total d'IA dans la partie
  unsigned int ai;
  /// @brief Le nombre maximal de tour de la partie
  /// @details Si max_turns = 0 alors la partie n'a pas de limite de tours
  unsigned int max_turns;
  /// @brief Les extensions utilisés pour la partie
  extension_vector_t extensions;
  /// @brief Le nombre d'itérations de l'algo mcts
  unsigned int ai_iterations;
};

/// @brief Lis les options depuis l'entrée standard
/// @param argc Le nombre d'arguments fournis
/// @param argv Les arguments fournis
/// @return Les options pour la partie à jouer
options_t parse_options(int argc, char* argv[]);

/// @brief Libère la mémoire d'une instance d'options_t
/// @param  options L'instance à libérer
void free_options(options_t* options);

/// @brief Vérifie la validité des options fournies
/// @param config L'instance d'option à vérifier
/// @return NULL si les options sont valides, un message d'erreur sinon
char* validate_options(options_t* config);

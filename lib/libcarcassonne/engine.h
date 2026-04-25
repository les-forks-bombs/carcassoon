#pragma once

#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine_state.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>

/// @brief Représente un moteur pour une partie de carcassonne
typedef struct engine {
  /// @brief La statut du plateau de jeu
  game_t         game;
  options_t      config;
  engine_state_t state;
} engine_t;

/// @brief Permet de créer une instance d'un moteur
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t create_engine(engine_t *engine, options_t options);

/// @brief Permet de désallouer une instance de moteur
/// @param engine Le moteur à détruire
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t destroy_engine(engine_t *engine);

/// @brief Permet de commencer une partie
/// @param engine Le moteur de jeu qui utilisera la partie
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t start_game(engine_t *engine);

/// @brief Permet au joueur courant d'appliquer une action sur la partie
/// @param engine Le moteur de jeu qui appliquera l'action
/// @param action L'action à appliquer
/// @attention Si le dernier joueur demande à mettre fin à son tour, le moteur
/// passe au tour suivant
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t dispatch_action(engine_t *engine, action_t action);

/// @brief Permet de récupérer l'état interne du moteur
/// @param engine Le moteur duquel récupérer l'état
/// @return L'état interne du moteur
engine_state_t get_engine_state(engine_t *engine);

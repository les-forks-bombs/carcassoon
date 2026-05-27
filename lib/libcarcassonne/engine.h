#pragma once

#include <libcarcassonne/forward.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>

/// @brief Représente un moteur pour une partie de carcassonne
struct engine {
  /// @brief La statut du plateau de jeu
  game_t                           game;
  options_t                        config;
  extension_process_hooks_vector_t hooks;
  unsigned int                     current_hook;
  dispatch_vector_t                dispatchs;
};

/// @brief Permet de créer une instance d'un moteur
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
return_code_t create_engine(engine_t *engine, options_t options);

/// @brief Permet de désallouer une instance de moteur
/// @param engine Le moteur à détruire
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
return_code_t destroy_engine(engine_t *engine);

/// @brief Permet de commencer une partie
/// @param engine Le moteur de jeu qui utilisera la partie
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
return_code_t start_game(engine_t *engine);

/// @brief Permet au joueur courant d'appliquer une action sur la partie
/// @details Le moteur exécutera autant de hook que possible jusqu'à ce qu'il ai besoin d'une nouvelle action pour continuer
/// @param engine Le moteur de jeu qui appliquera l'action
/// @param action L'action à appliquer
/// @attention Si le dernier joueur demande à mettre fin à son tour, le moteur
/// passe au tour suivant
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
/// @relates extension_process_hook_t
/// @relates dispatch_t
return_code_t dispatch_action(engine_t *engine, action_t action);

/// @brief Annules toutes les actions jusqu'à avant l'action n° epoch
/// @details Ex: revert(0) permet de remettre le jeu à 0. Donc dans son état juste après start_game.
/// @details Ainsi, revert(n) revient avant la n-ième action, le moteur demandera donc l'action numéro n.
/// @details Le hook courant sera mis à jour.
/// @param engine Le moteur de jeu qui annulera les actions de sa partie
/// @param epoch Le n° d'action auquel retourner
/// @attention Le moteur va véritablement annuler et détruire les actions.
/// Si vous souhaitez les rejouer, il faudra les conserver vous-mêmes.
/// @return Le code de statut, SUCCESS si l'opération a réussi, autre sinon
/// @relates extension_process_hook_t
/// @relates dispatch_t
return_code_t engine_revert(engine_t *engine, unsigned int epoch);

/// @brief Permet de récupérer l'action attendue par le hook courant
/// @param engine Le moteur de jeu
/// @return Le type d'action attendu par le prochain hook à exécuter
/// @relates extension_process_hook_t
/// @relates action_t
action_type_t engine_wanted_action(engine_t *engine);

/// @brief Permet de récupérer la liste des actions proposés par le hook courant
/// @details Renvoi le retour de <hook_name>_list_actions
/// @param engine Le moteur de jeu
/// @return La liste des actions que propose le hook courant
/// @relates extension_process_hook_t
/// @relates action_t
action_vector_t engine_get_actions(engine_t *engine);
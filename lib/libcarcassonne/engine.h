#ifndef H_LIBCARCASSONNE_ENGINE
#define H_LIBCARCASSONNE_ENGINE

#include <libcarcassonne/action.h>
#include <libcarcassonne/deck.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>

/// @brief Représente la prochaine action attendue par le moteur
typedef enum engine_state
{
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_TILE_ACTION = LIBCARCASSONNE_ACTION_PLACE_TILE,
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION = LIBCARCASSONNE_ACTION_PLACE_MEEPLE,
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_END_TURN = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_END_TURN,
} engine_state_t;

/// @brief Représente un moteur pour une partie de carcassonne
typedef struct engine
{
  /// @brief La statut du plateau de jeu
  game_t game;
} engine_t;

/// @brief Permet de créer une instance d'un moteur
/// @return Une instance de moteur
engine_t create_engine();

/// @brief Permet de désallouer une instance de moteur
/// @param engine Le moteur à détruire
void destroy_engine(engine_t *engine);

/// @brief Pemret de commencer une partie
/// @param engine Le moteur de jeu qui utilisera la partie
/// @param players_count Nombre de joueur
/// @param ai_count Nombre de joueur IA
/// @param seed Graine aléatoire
/// @param turns_limit Nombre maximum de tours
void start_game(engine_t *engine, unsigned int players_count, unsigned int ai_count, unsigned int seed, unsigned int turns_limit);

/// @brief Permet au joueur courant d'appliquer une action sur la partie
/// @param engine Le moteur de jeu qui appliquera l'action
/// @param action L'action à appliquer
/// @return Status de l'action
return_code_t dispatch_action(engine_t *engine, action_t action);

/// @brief Réalise un tour de jeu complet
void play_turn();

/// @brief Permet de récupérer l'état interne du moteur
/// @param engine Le moteur duquel récupérer l'état
/// @return L'état interne du moteur
engine_state_t get_engine_state(engine_t *engine);

#endif

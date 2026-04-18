#ifndef H_LIBCARCASSONNE_ENGINE
#define H_LIBCARCASSONNE_ENGINE

#include <libcarcassonne/deck.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>

/// @brief Représente un moteur pour une partie de carcassonne
typedef struct engine {
  /// @brief La statut du plateau de jeu
  game_t game;
  /// @brief Les options de configuration
  options_t config;
} engine_t;

return_code_t create_engine(engine_t *engine, options_t options);
void          destroy_engine(engine_t *engine);

#endif

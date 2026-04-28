#pragma once

/// @brief Représente la prochaine action attendue par le moteur
typedef enum engine_state {
  LIBCARCASSONNE_ENGINE_NULL_ENGINE                  = -1,
  LIBCARCASSONNE_ENGINE_WAITING_GAME_START           = 0,
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_TILE_ACTION   = 1,
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION = 2,
  LIBCARCASSONNE_ENGINE_WAITING_PLAYER_END_TURN      = 3,
} engine_state_t;

#pragma once

#include <libcarcassonne/forward.h>

/// @brief Représente un événement de jeu
struct dispatch {
  /// @brief Stockage générique pour permettre l'annulation de l'action
  void* state_store;
  /// @brief L'action exécutée par le hook
  action_t* action;
  /// @brief Le hook utilisé pour cet événement
  const extension_process_hook_t* hook;
};

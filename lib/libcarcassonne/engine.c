#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/game.h>

return_code_t create_engine(engine_t *engine, options_t options) {
  if (engine == NULL) {
    return ERROR;
  }

  engine->state = LIBCARCASSONNE_ENGINE_WAITING_GAME_START;

  engine->config = options;

  return_code_t code = create_game(&engine->game, &engine->config);
  if (code != SUCCESS) {
    return code;
  }

  return SUCCESS;
}

return_code_t destroy_engine(engine_t *engine) {
  if (engine == NULL) {
    return NULL_POINTER;
  }

  destroy_game(&engine->game);

  return SUCCESS;
}

return_code_t start_game(engine_t *engine) {
  if (engine == NULL) return NULL_POINTER;

  return_code_t code =
      game_place_tile(&engine->game, deck_pick(&engine->game.deck), 0, 0,
                      LIBCARCASSONNE_TILE_ORIENTATION_NORTH);

  if (code != SUCCESS) {
    return code;
  }

  engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_TILE_ACTION;
}

/// @brief Gère le placement d'une tuile
/// @param engine Le moteur auquel on demande de placer une tuile
/// @param action L'action à effectuer
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_place_tile(engine_t *engine, action_t action) {
  game_t *game = &engine->game;

  tile_t            *tile        = action.order.place_tile.tile;
  int                x           = action.order.place_tile.x;
  int                y           = action.order.place_tile.y;
  tile_orientation_t orientation = action.order.place_tile.orientation;

  return_code_t code = game_place_tile(game, tile, x, y, orientation);

  if (code == SUCCESS) {
    engine->state = LIBCARCASSONNE_ENGINE_WAITING_PLAYER_MEEPLE_ACTION;
  }
  return code;
}

/// @brief Gère le placement d'un meeple
/// @param engine Le moteur auquel on demande de placer une tuile
/// @param action L'action à effectuer
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_place_meeple(engine_t *engine, action_t action) {
  game_t *game = &engine->game;

  int           part_group = action.order.place_meeple.part_group;
  int           x          = action.order.place_meeple.x;
  int           y          = action.order.place_meeple.y;
  meeple_type_t type       = action.order.place_meeple.meeple_type;

  return game_place_meeple(game, x, y, part_group, type);
}

/// @brief Gère la fin du tour d'un joueur
/// @param engine Le moteur auquel on demande de placer une tuile
/// @param action L'action à effectuer
/// @warning En cas de réussite le moteur change d'état
/// @return Le code de statut, SUCCESS si l'opération a réussit, autre sinon
return_code_t handle_end_player_turn(engine_t *engine, action_t action) {
  return_code_t code = game_end_player_turn(&engine->game);
  if (code == NO_MORE_PLAYER)  // Si on a plus de joueur à faire jouer on passe
                               // au round suivant
    return game_end_round(&engine->game);
  else  // Sinon, on renvoie le code de succès ou d'erreur
    return code;
}

return_code_t dispatch_action(engine_t *engine, action_t action) {
  if (engine == NULL) return NULL_POINTER;

  if (engine->state == LIBCARCASSONNE_ENGINE_WAITING_GAME_START ||
      engine->state != action.type) {
    return INVALID_ACTION;
  }

  switch (action.type) {
    case LIBCARCASSONNE_ACTION_PLACE_TILE:
      return handle_place_tile(engine, action);
      break;
    case LIBCARCASSONNE_ACTION_PLACE_MEEPLE:
      return handle_place_meeple(engine, action);
      break;
    case LIBCARCASSONNE_ACTION_END_PLAYER_TURN:
      return handle_end_player_turn(engine, action);
      break;
    default:
      return INVALID_ACTION;
  }

  return INVALID_ACTION;
}

engine_state_t get_engine_state(engine_t *engine) {
  if (engine == NULL) return LIBCARCASSONNE_ENGINE_NULL_ENGINE;
  return engine->state;
}

#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine.h>
#include <libcarcassonne/game.h>

return_code_t create_engine(engine_t *engine, options_t options) {
  if (engine == NULL) {
    return ERROR;
  }

  engine->config = options;

  if (create_game(&engine->game, &engine->config) != SUCCESS) {
    return ERROR;
  }

  return SUCCESS;
}

void destroy_engine(engine_t *engine) {
  if (engine == NULL) {
    return;
  }

  destroy_game(&engine->game);
}

#include <libcarcassonne/engine.h>
#include <stdio.h>

#include "libcarcassonne/options.h"

int main(int argc, char* argv[]) {
  options_t config = parse_options(argc, argv);
  char*     reason = validate_options(&config);
  if (reason != NULL) {
    printf("Configuration invalide: %s\n", reason);
    return 1;
  }

  engine_t engine;
  create_engine(&engine, config);

  switch (engine.config.mode) {
    case CARCASSONNE_MODE_CLI:
      printf("Starting using CLI...\n");
      break;
    case CARCASSONNE_MODE_SDL:
      printf("Starting using SDL...\n");

      break;
    default:
      break;
  }

  destroy_engine(&engine);
  free_options(&config);
}

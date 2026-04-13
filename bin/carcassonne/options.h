#include <stdbool.h>

typedef enum mode {
  CARCASSONNE_MODE_SDL = 0,
  CARCASSONNE_MODE_CLI = 1,
} mode_t;

typedef struct runtime_config {
  mode_t       mode;
  unsigned int players;
  unsigned int seed;
  unsigned int ai;
  unsigned int max_turns;
} runtime_config_t;

runtime_config_t parse_options(
    int argc,
    char* argv[]
);

char* validate_options(
    runtime_config_t*
);

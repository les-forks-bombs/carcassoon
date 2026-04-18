#pragma once

#include <getopt.h>
#include <unistd.h>

#include "libcarcassonne/extension.h"

typedef enum runtime_mode {
  CARCASSONNE_MODE_SDL = 0,
  CARCASSONNE_MODE_CLI = 1,
} options_mode_t;

typedef struct runtime_config {
  options_mode_t   mode;
  unsigned int     players;
  unsigned int     seed;
  unsigned int     ai;
  unsigned int     max_turns;
  extension_list_t extensions;
} options_t;

options_t parse_options(int argc, char* argv[]);

char* validate_options(options_t*);

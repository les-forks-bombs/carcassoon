#pragma once

#include <getopt.h>
#include <libcarcassonne/extension.h>
#include <unistd.h>

#include "libcarcassonne/forward.h"

typedef enum options_mode {
  CARCASSONNE_MODE_SDL = 0,
  CARCASSONNE_MODE_CLI = 1,
} options_mode_t;

struct options {
  options_mode_t     mode;
  unsigned int       players;
  unsigned int       seed;
  unsigned int       ai;
  unsigned int       max_turns;
  extension_vector_t extensions;
};

options_t parse_options(int argc, char* argv[]);

void free_options(options_t*);

char* validate_options(options_t*);

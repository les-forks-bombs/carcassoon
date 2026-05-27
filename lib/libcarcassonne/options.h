#pragma once

#include <libcarcassonne/enums.h>
#include <libcarcassonne/extension.h>
#include <unistd.h>

struct options {
  options_mode_t     mode;
  unsigned int       players;
  int                seed;
  unsigned int       ai;
  unsigned int       max_turns;
  extension_vector_t extensions;
};

options_t parse_options(int argc, char* argv[]);

void free_options(options_t* /*options*/);

char* validate_options(options_t* /*config*/);

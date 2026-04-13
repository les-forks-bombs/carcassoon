#include "options.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

runtime_config_t parse_options(int argc, char* argv[]) {
  int c;

  runtime_config_t config = {.ai        = 0,
                             .max_turns = 0,
                             .seed      = time(NULL),
                             .players   = 3,
                             .mode      = CARCASSONNE_MODE_SDL};

  while (1) {
    int           option_index = 0;
    char*         endPtr;
    int           value;
    struct option long_options[] = {{"mode", required_argument, 0, 'm'},
                                    {"players", required_argument, 0, 'p'},
                                    {"ai", required_argument, 0, 'a'},
                                    {"max-turns", required_argument, 0, 't'},
                                    {"seed", required_argument, 0, 's'},
                                    {0, 0, 0, 0}};

    c = getopt_long(argc, argv, "m:p:a:t:s:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'm':
        if (strcmp("sdl", optarg) == 0) {
            config.mode = CARCASSONNE_MODE_SDL;
        }
        if (strcmp("cli", optarg) == 0) {
            config.mode = CARCASSONNE_MODE_CLI;
        }

        break;
      case 'p':
        config.players = strtod(optarg, &endPtr);
        break;
      case 'a':
        config.ai = strtod(optarg, &endPtr);
        break;
      case 't':
        config.max_turns = strtod(optarg, &endPtr);
        break;
      case 's':
        config.seed = strtod(optarg, &endPtr);
        break;

      default:

        printf("option %s (%c)", long_options[option_index].name, c);
        if (optarg) printf(" with arg %s", optarg);
        printf("\n");
        break;
    }
  }

  return config;
}

char* validate_options(runtime_config_t* config) {
    if (config->ai >= config->players) {
        return "Le nombre d'IA est supérieur au nombre de joueurs!";
    }

    return NULL;
}

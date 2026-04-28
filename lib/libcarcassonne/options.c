#include <getopt.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/extensions_list.h>
#include <libcarcassonne/options.h>
#include <libutils/vector.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libcarcassonne/forward.h"

const char* help_string =
    ""
    ""
    "Usage: %1$s [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -m, --mode=MODE          Set the mode (sdl or cli)\n"
    "  -p, --players=NUM        Set the number of players\n"
    "  -a, --ai=NUM             Set the number of AI players\n"
    "  -t, --max-turns=NUM      Set the maximum number of turns\n"
    "  -s, --seed=NUM           Set the random seed\n"
    "  -e, --extensions=A       Set of extensions to enable (use this option "
    "multiple times to enable many extensions)\n"
    "\n"
    "Example:\n"
    "  %1$s -m sdl -p 4 -a 2 -t 100 -s 12345\n"
    "  %1$s -m cli -p 4 -a 2 -t 100 -s 12345\n"
    "\n"
    "Attribution:"
    "  Matthieu P. & Salomé P. & Damien J. & Bastien VD.\n"
    "\n";

options_t parse_options(int argc, char* argv[]) {
  int  c;
  char arg[25] = "";

  options_t config = {
      .ai         = 0,
      .max_turns  = 0,
      .seed       = time(NULL),
      .players    = 3,
      .mode       = CARCASSONNE_MODE_SDL,
      .extensions = {0},
  };

  memset(&config.extensions, 0, sizeof(extension_vector_t));

  extension_t* ext = (extension_t*)&LIBCARCASSONNE_EXT_BASE_GAME;
  vector_append(&config.extensions, &ext);

  while (1) {
    int           option_index = 0;
    char*         endPtr;
    struct option long_options[] = {{"mode", required_argument, 0, 'm'},
                                    {"players", required_argument, 0, 'p'},
                                    {"ai", required_argument, 0, 'a'},
                                    {"max-turns", required_argument, 0, 't'},
                                    {"seed", required_argument, 0, 's'},
                                    {"help", no_argument, 0, 'h'},
                                    {"extension", required_argument, 0, 'e'},
                                    {0, 0, 0, 0}};

    c = getopt_long(argc, argv, "m:p:a:t:s:he:", long_options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 'm':
        if (strcmp("sdl", optarg) == 0) {
          config.mode = CARCASSONNE_MODE_SDL;
        } else if (strcmp("cli", optarg) == 0) {
          config.mode = CARCASSONNE_MODE_CLI;
        } else {
          goto bad_value;
        }

        break;
      case 'p':
        config.players = strtod(optarg, &endPtr);
        if (endPtr == optarg) goto bad_value;

        break;
      case 'a':
        config.ai = strtod(optarg, &endPtr);
        if (endPtr == optarg) goto bad_value;
        break;
      case 't':
        config.max_turns = strtod(optarg, &endPtr);
        if (endPtr == optarg) goto bad_value;
        break;
      case 's':
        config.seed = strtod(optarg, &endPtr);
        if (endPtr == optarg) goto bad_value;
        break;
      case 'e':
        // trouver l'extension
        for (unsigned int i = 0; i < sizeof(LIBCARCASSONNE_EXTENSIONS) /
                                         sizeof(LIBCARCASSONNE_EXTENSIONS[0]);
             i++) {
          if (strcmp(LIBCARCASSONNE_EXTENSIONS[i]->name, optarg) == 0) {
            // todo: implement append to extension_list_t
            printf("Ajout de l'extension %s \n",
                   LIBCARCASSONNE_EXTENSIONS[i]->name);
            goto e_success;
            break;
          }
        }
        goto bad_value;

      e_success:
        break;

      bad_value:
        if (long_options[option_index].has_arg == 0) {
          sprintf(&arg[0], "-%c", c);
        } else {
          sprintf(&arg[0], "--%s", long_options[option_index].name);
        }
        printf("La valeur pour %s, '%s' n'est pas une valeur valide.\n\n", arg,
               optarg);
      case 'h':
      print_help:
      default:
        printf(help_string, argv[0]);
        exit(0);
        break;
    }
  }
  char* message;

  if ((message = validate_options(&config)) != NULL) {
    printf("Les paramètres sont invalide: %s\n\n", message);
    goto print_help;
  }

  return config;
}

char* validate_options(options_t* config) {
  if (config->ai >= config->players) {
    return "Le nombre d'IA est supérieur au nombre de joueurs!";
  }

  if (config->players < 2) {
    return "Le nombre de joueurs doit être au moins égal à 2!";
  }

  if (config->players >= LIBCARCASSONNE_MAX_PLAYERS) {
    return "Le nombre de joueurs doit être inférieur à 5!";
  }

  if (vector_size(&config->extensions) == 0) {
    return "Pour jouer a carcassonne, vous devez avoir au minimum l'extension "
           "de base.";
  }

  // Vérification des dépendances
  for (unsigned int i = 0; i < vector_size(&config->extensions); i++) {
    for (unsigned int j = 0;
         j < vector_size((*vector_nth(&config->extensions, i))->required);
         j++) {
      for (unsigned int k = 0; k < vector_size(&config->extensions); k++) {
        const char* name =
            (*vector_nth((*vector_nth(&config->extensions, i))->required, k))
                ->name;

        if ((*vector_nth(&config->extensions, k)) ==
            (*vector_nth((*vector_nth(&config->extensions, i))->required, j))) {
          goto found;
        }

        printf("Extension %s requise par %s\n", name,
               (*vector_nth(&config->extensions, i))->name);

        return "Une extension requise n'est pas présente dans la liste des "
               "extensions!";

      found:
        continue;
      }
    }
  }

  return NULL;
}

void free_options(options_t* t) { vector_free(&t->extensions); }

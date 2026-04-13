#include "options.h"
#include <stdio.h>
#include <libcarcassonne/libcarcassonne.h>

int main(int argc, char* argv[]) {
    runtime_config_t config = parse_options(argc, argv);

    char* reason = validate_options(&config);
    if (reason != NULL) {
        printf("Configuration invalide: %s\n", reason);
        return 1;
    }

    game_t game;
    create_game(&game, config.players, config.ai, config.seed, config.max_turns);
    
    switch (config.mode)
    {
    case CARCASSONNE_MODE_CLI:
        printf("Starting using CLI...\n");
        break;
    case CARCASSONNE_MODE_SDL:
        printf("Starting using SDL...\n");
        
        break;
    default:
        break;
    }

    destroy_game(&game);
}

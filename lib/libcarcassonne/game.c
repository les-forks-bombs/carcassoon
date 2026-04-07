#include <libcarcassonne/game.h>

#include <assert.h>
#include <libcarcassonne/consts.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

return_code_t create_game(
    game_t *game,
    unsigned int players_count,
    unsigned int ai_count,
    unsigned int seed,
    unsigned int turns_limit)
{
    // On vérifie que le nombre de joueurs est dans [2, 5]
    // et que le nombre d'IA est inférieur au nombre de joueurs
    if (
        players_count < 2 || players_count > 5 || ai_count > players_count)
    {
        return ERROR;
    }

    game->current_player = 0;
    game->deck = create_deck(seed);
    game->players_count = players_count;
    game->turn = 0;
    game->turns_limit = turns_limit;

    // todo: considérer les extensions pour calculer la taille max du tableau
    unsigned int largeur = LIBCARCASSONNE_TILES_COUNT + LIBCARCASSONNE_TILES_COUNT - 1;

    game->map = calloc(
        largeur * largeur,
        sizeof(placed_tile_t *));

    // on instancie les joueurs
    for (unsigned int i = 0; i < players_count; i++)
        game->players[i] = create_player(
            i > ai_count ? LIBCARCASSONNE_PLAYER_HUMAN : LIBCARCASSONNE_PLAYER_AI);

    return SUCCESS;
}

void destroy_game(game_t *game)
{
    // Cleanup des structures dans la map (les placed_tile_t)
    for (int i = -LIBCARCASSONNE_TILES_COUNT + 1; i < LIBCARCASSONNE_TILES_COUNT; i++)
        for (int j = -LIBCARCASSONNE_TILES_COUNT + 1; j < LIBCARCASSONNE_TILES_COUNT; j++)
        {
            placed_tile_t **tile = game_tile_at(game, i, j);
            if (*tile != NULL)
                free(*tile);
        }

    free(game->map);
    free_deck(game->deck);
}

placed_tile_t **game_tile_at(
    game_t *game,
    int colonne,
    int ligne)
{
    int zero = LIBCARCASSONNE_TILES_COUNT - 1;
    int size = LIBCARCASSONNE_TILES_COUNT * 2 - 1;

    int index = (zero + ligne) * size + (zero + colonne);

    return &game->map[index];
}

return_code_t game_place_tile(
    game_t *game,
    tile_t *tile,
    int x,
    int y)
{
    if (x <= -LIBCARCASSONNE_TILES_COUNT ||
        y <= -LIBCARCASSONNE_TILES_COUNT ||
        x >= LIBCARCASSONNE_TILES_COUNT ||
        y >= LIBCARCASSONNE_TILES_COUNT)
    {
        return OUT_OF_BOUNDS; // Out of bounds
    }

    placed_tile_t **tile_ref = game_tile_at(game, x, y);

    if (*tile_ref == NULL)
    {
        placed_tile_t *placed_tile = calloc(1, sizeof(placed_tile_t));
        placed_tile->parent = tile;

        // todo: implémenter les meeple
        placed_tile->meeple = NULL;

        *tile_ref = placed_tile;
        return SUCCESS; // Placed
    }
    else
    {
        return NOT_FREE; // not Free
    }
}

void game_print_map(game_t *game)
{
    for (int i = -LIBCARCASSONNE_TILES_COUNT + 1; i < LIBCARCASSONNE_TILES_COUNT; i++)
    {
        printf("|");
        for (int j = -LIBCARCASSONNE_TILES_COUNT + 1; j < LIBCARCASSONNE_TILES_COUNT; j++)
        {
            placed_tile_t **tile = game_tile_at(game, i, j);
            if (*tile == NULL)
                printf(".");
            else
                printf("#");
        }
        printf("|\n");
    }
}

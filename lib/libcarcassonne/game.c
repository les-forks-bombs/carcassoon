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
    if (game == NULL)
    {
        return ERROR;
    }

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
    if (game == NULL)
    {
        return;
    }

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
    if (colonne <= -LIBCARCASSONNE_TILES_COUNT ||
        ligne <= -LIBCARCASSONNE_TILES_COUNT ||
        colonne >= LIBCARCASSONNE_TILES_COUNT ||
        ligne >= LIBCARCASSONNE_TILES_COUNT)
    {
        return NULL; // Out of bounds
    }

    int zero = LIBCARCASSONNE_TILES_COUNT - 1;
    int size = LIBCARCASSONNE_TILES_COUNT * 2 - 1;

    int index = (zero + ligne) * size + (zero + colonne);

    return &game->map[index];
}

return_code_t game_place_tile(
    game_t *game,
    tile_t *tile,
    int x,
    int y,
    tile_orientation_t orientation)
{
    if (game == NULL || tile == NULL)
    {
        return ERROR;
    }

    placed_tile_t **tile_ref = game_tile_at(game, x, y);

    if (tile_ref == NULL)
    {
        return OUT_OF_BOUNDS; // Out of bounds
    }

    if (*tile_ref == NULL)
    {

        if (!game_is_tile_placeable(game, tile, x, y, orientation))
            return INVALID_PLACEMENT;

        placed_tile_t *placed_tile = calloc(1, sizeof(placed_tile_t));
        placed_tile->parent = tile;
        placed_tile->orientation = orientation;

        *tile_ref = placed_tile;
        return SUCCESS; // Placed
    }
    else
    {
        return NOT_FREE; // not Free
    }
}

return_code_t game_place_meeple(
    game_t *game,
    int x,
    int y,
    int tile_part)
{
    if (game == NULL)
    {
        return ERROR;
    }

    placed_tile_t **tile_ref = game_tile_at(game, x, y);

    if (tile_ref == NULL)
    {
        return OUT_OF_BOUNDS; // Out of bounds
    }

    if (*tile_ref != NULL)
    {
        (*tile_ref)->meeple[tile_part] = calloc(1, sizeof(meeple_t));
        (*tile_ref)->meeple[tile_part]->player = game->current_player;
    }
    else
    {
        return NO_TILE;
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

bool game_is_tile_placeable(game_t *game, tile_t *tile, int x, int y, tile_orientation_t orientation)
{
    if (game == NULL || tile == NULL)
        return false;

    if (x == 0 && y == 0)
        return true;

    placed_tile_t **up_tile = game_tile_at(game, x - 1, y);
    placed_tile_t **down_tile = game_tile_at(game, x + 1, y);
    placed_tile_t **left_tile = game_tile_at(game, x, y - 1);
    placed_tile_t **right_tile = game_tile_at(game, x, y + 1);

    if (*up_tile == NULL && *down_tile == NULL && *left_tile == NULL && *right_tile == NULL)
        return false;

    if (*up_tile != NULL && tile_get_family_face(tile, orientation, LIBCARCASSONNE_TILE_ORIENTATION_NORTH) != tile_get_family_face((*up_tile)->parent, (*up_tile)->orientation, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH))
    {
        return false;
    }

    if (*down_tile != NULL && tile_get_family_face(tile, orientation, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH) != tile_get_family_face((*down_tile)->parent, (*down_tile)->orientation, LIBCARCASSONNE_TILE_ORIENTATION_NORTH))
    {
        return false;
    }

    if (*left_tile != NULL && tile_get_family_face(tile, orientation, LIBCARCASSONNE_TILE_ORIENTATION_WEST) != tile_get_family_face((*left_tile)->parent, (*left_tile)->orientation, LIBCARCASSONNE_TILE_ORIENTATION_EAST))
    {
        return false;
    }

    if (*right_tile != NULL && tile_get_family_face(tile, orientation, LIBCARCASSONNE_TILE_ORIENTATION_EAST) != tile_get_family_face((*right_tile)->parent, (*right_tile)->orientation, LIBCARCASSONNE_TILE_ORIENTATION_WEST))
    {
        return false;
    }

    return true;
}
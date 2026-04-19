#include <assert.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/game.h>
#include <libcarcassonne/options.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

return_code_t create_game(game_t *game, options_t *options)
{
  if (game == NULL)
  {
    return ERROR;
  }

  if (validate_options(options) != NULL)
  {
    return ERROR;
  }

  game->current_player = 0;
  game->deck = create_deck(options->seed, &options->extensions);
  game->options = options;

  game->open_tiles = create_open_tiles_list();

  // todo: considérer les extensions pour calculer la taille max du tableau
  unsigned int largeur =
      LIBCARCASSONNE_TILES_COUNT + LIBCARCASSONNE_TILES_COUNT - 1;

  game->map = calloc(largeur * largeur, sizeof(placed_tile_t *));

  // on instancie les joueurs
  for (unsigned int i = 0; i < game->options->players; i++)
    game->players[i] =
        create_player(i > game->options->ai ? LIBCARCASSONNE_PLAYER_HUMAN
                                            : LIBCARCASSONNE_PLAYER_AI);

  return SUCCESS;
}

void destroy_game(game_t *game)
{
  if (game == NULL)
  {
    return;
  }

  // Cleanup des structures dans la map (les placed_tile_t)
  for (int i = -LIBCARCASSONNE_TILES_COUNT + 1; i < LIBCARCASSONNE_TILES_COUNT;
       i++)
    for (int j = -LIBCARCASSONNE_TILES_COUNT + 1;
         j < LIBCARCASSONNE_TILES_COUNT; j++)
    {
      placed_tile_t **tile = game_tile_at(game, i, j);
      if (*tile != NULL)
      {
        placed_tile_destroy(*tile);
        free(*tile);
      }
    }

  destroy_tile_list(&game->open_tiles);

  free(game->map);
  free_deck(game->deck);
}

placed_tile_t **game_tile_at(game_t *game, int colonne, int ligne)
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

return_code_t game_place_tile(game_t *game, const tile_t *tile, int x, int y,
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
    placed_tile_create(placed_tile, tile, orientation);

    *tile_ref = placed_tile;

    if (game_is_place_open(game, x, y))
      game_add_open_tile(&game->open_tiles, placed_tile);

    if (!game_is_place_open(game, x + 1, y)) // Tuile du bas
      game_remove_open_tile(&game->open_tiles, *game_tile_at(game, x + 1, y));

    if (!game_is_place_open(game, x - 1, y)) // Tuile du haut
      game_remove_open_tile(&game->open_tiles, *game_tile_at(game, x - 1, y));

    if (!game_is_place_open(game, x, y + 1)) // Tuile de droite
      game_remove_open_tile(&game->open_tiles, *game_tile_at(game, x, y + 1));

    if (!game_is_place_open(game, x, y - 1)) // Tuile de gauche
      game_remove_open_tile(&game->open_tiles, *game_tile_at(game, x, y - 1));

    for (tile_orientation_t s = 0; s < 4; s++)
    {
      placed_tile_t **neighbor;

      switch (s)
      {
      case LIBCARCASSONNE_TILE_ORIENTATION_NORTH:
        neighbor = game_tile_at(game, x - 1, y);
        break;
      case LIBCARCASSONNE_TILE_ORIENTATION_SOUTH:
        neighbor = game_tile_at(game, x + 1, y);
        break;
      case LIBCARCASSONNE_TILE_ORIENTATION_EAST:
        neighbor = game_tile_at(game, x, y + 1);
        break;
      case LIBCARCASSONNE_TILE_ORIENTATION_WEST:
        neighbor = game_tile_at(game, x, y - 1);
        break;
      }

      // on se marque comme étant voisin de notre voisin, et réciproquement
      if (neighbor != NULL && *neighbor != NULL)
      {
        placed_face_groups_t neighboor_face;
        placed_face_groups_t current_face;
        placed_tile_get_face(&neighboor_face, *neighbor,
                             tile_orientation_invert(s));
        placed_tile_get_face(&current_face, placed_tile, s);

        for (int i = 0; i < 3; i++)
        {
          placed_tile_group_link(neighboor_face.face[i], current_face.face[i]);
        }
      }
    }

    return SUCCESS; // Placed
  }
  else
  {
    return NOT_FREE; // not Free
  }
}

return_code_t game_place_meeple(game_t *game, int x, int y, int group)
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
    placed_tile_group_t *group_ref = (*tile_ref)->groups[group];

    if (group_ref->meeple == NULL)
    {
      group_ref->meeple = calloc(1, sizeof(meeple_t));
      group_ref->meeple->player = game->current_player;
      group_ref->meeple->group_node = group_ref;

      // todo: ajouter au meeple a la liste des meeple
    }
    else
    {
      return ALREADY_ALLOCATED;
    }
    return SUCCESS;
  }
  else
  {
    return NO_TILE;
  }
}

bool game_is_tile_placeable(game_t *game, const tile_t *tile, int x, int y,
                            tile_orientation_t orientation)
{
  if (game == NULL || tile == NULL)
    return false;

  if (x == 0 && y == 0)
    return true;

  placed_tile_t **up_tile = game_tile_at(game, x - 1, y);
  placed_tile_t **down_tile = game_tile_at(game, x + 1, y);
  placed_tile_t **left_tile = game_tile_at(game, x, y - 1);
  placed_tile_t **right_tile = game_tile_at(game, x, y + 1);

  if ((up_tile == NULL || *up_tile == NULL) &&
      (down_tile == NULL || *down_tile == NULL) &&
      (left_tile == NULL || *left_tile == NULL) &&
      (right_tile == NULL || *right_tile == NULL))
    return false;

  if (*up_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH) !=
          tile_get_family_face((*up_tile)->parent, (*up_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_SOUTH))
  {
    return false;
  }

  if (down_tile != NULL && *down_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH) !=
          tile_get_family_face((*down_tile)->parent, (*down_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_NORTH))
  {
    return false;
  }

  if (left_tile != NULL && *left_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST) !=
          tile_get_family_face((*left_tile)->parent, (*left_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_EAST))
  {
    return false;
  }

  if (right_tile != NULL && *right_tile != NULL &&
      tile_get_family_face(tile, orientation,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST) !=
          tile_get_family_face((*right_tile)->parent,
                               (*right_tile)->orientation,
                               LIBCARCASSONNE_TILE_ORIENTATION_WEST))
  {
    return false;
  }

  return true;
}

void destroy_tile_list(tile_list_t *tl)
{
  tile_list_element_t *next = tl->head;
  while (tl->head != NULL)
  {
    next = tl->head;
    tl->head->next = NULL;
    tl->head->tile = NULL;

    free(tl->head);

    tl->head = next;
  }
}

bool game_is_place_open(game_t *game, int x, int y)
{
  if (game_tile_at(game, x + 1, y) != NULL &&
      game_tile_at(game, x - 1, y) != NULL &&
      game_tile_at(game, x, y + 1) != NULL &&
      game_tile_at(game, x, y - 1) != NULL)
    return false;

  return true;
}

void game_add_open_tile(tile_list_t *tl, placed_tile_t *tile)
{
  tile_list_element_t *head = malloc(sizeof(tile_list_element_t));
  head->tile = tile;
  head->next = tl->head;
  tl->head = head;

  if (tl->tail == NULL)
    tl->tail = head;

  tl->size++;
}

void game_remove_open_tile(tile_list_t *tl, placed_tile_t *tile)
{
  tile_list_element_t *curr = tl->head;
  while (curr != NULL)
  {
    if (curr->tile == tile)
    {
      if (tl->head == curr)
        tl->head = curr->next;

      if (tl->tail == curr)
        tl->tail = curr->prev;

      curr->prev->next = curr->next;
      curr->next->prev = curr->prev;
      curr->tile = NULL;
      free(curr);

      tl->size--;
      return;
    }
  }
}

tile_list_t create_open_tiles_list(void)
{
  tile_list_t tl;
  tl.head = NULL;
  tl.tail = 0;
  tl.size = 0;

  return tl;
}

bool is_game_finished(game_t *game)
{
  return game->deck.list.size == 0 || game->turn == game->turns_limit;
}

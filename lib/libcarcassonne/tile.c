#include "tile.h"

char tile_get_family_face(tile_t *tile, tile_orientation_t orientation)
{
    char family;

    if (orientation == LIBCARCASSONNE_TILE_ORIENTATION_NORTH)
        family = tile->family[0];
    else if (orientation == LIBCARCASSONNE_TILE_ORIENTATION_EAST)
        family = tile->family[1];
    else if (orientation == LIBCARCASSONNE_TILE_ORIENTATION_EAST)
        family = tile->family[2];
    else
        family = tile->family[3];

    return family;
}

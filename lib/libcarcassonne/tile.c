#include "tile.h"

#include <stdlib.h>

char tile_get_family_face(tile_t *tile, tile_orientation_t orientation, tile_orientation_t connexion_face)
{
    if (tile == NULL)
        return 0;

    int index = (orientation + connexion_face) % 4;
    return tile->family[index];
}

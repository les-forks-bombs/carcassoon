#ifndef H_LIBCARCASSONNE_TILE
#define H_LIBCARCASSONNE_TILE

#include <stdbool.h>

#define LIBCARCASSONNE_TILE_PART_VILLAGE 0
#define LIBCARCASSONNE_TILE_PART_CHAMP   1
#define LIBCARCASSONNE_TILE_PART_ROUTE   2
#define LIBCARCASSONNE_TILE_PART_MURS    3
#define LIBCARCASSONNE_TILE_PART_ABBEY   4
#define LIBCARCASSONNE_TILE_PART_TOWN    5

typedef char tile_type_t;

/// @brief A tile is a 3x3 of tile parts
typedef struct tile
{
    /// @brief The "name" of this tile
    char name[5];
    /// @brief Every tile is made of 9 tile parts
    tile_type_t parts[9];

    bool blason;
} tile_t;

#endif
#ifndef H_LIBCARCASSONNE_TILE
#define H_LIBCARCASSONNE_TILE

#include <stdbool.h>
#include <libcarcassonne/meeple.h>

#define LIBCARCASSONNE_TILE_PART_VILLAGE 0
#define LIBCARCASSONNE_TILE_PART_FIELD   1
#define LIBCARCASSONNE_TILE_PART_ROAD    2
#define LIBCARCASSONNE_TILE_PART_WALL    3
#define LIBCARCASSONNE_TILE_PART_ABBEY   4
#define LIBCARCASSONNE_TILE_PART_TOWN    5

#define LIBCARCASSONNE_TILE_PART_A 1 << 0
#define LIBCARCASSONNE_TILE_PART_B 1 << 1
#define LIBCARCASSONNE_TILE_PART_C 1 << 2
#define LIBCARCASSONNE_TILE_PART_D 1 << 3
#define LIBCARCASSONNE_TILE_PART_E 1 << 4
#define LIBCARCASSONNE_TILE_PART_F 1 << 5
#define LIBCARCASSONNE_TILE_PART_G 1 << 6
#define LIBCARCASSONNE_TILE_PART_H 1 << 7
#define LIBCARCASSONNE_TILE_PART_I 1 << 8

typedef char tile_part_type_t;
typedef char tile_part_group_t;

/// @brief Une tile est un élément de jeu de carcassonne
typedef struct tile
{
    /// @brief Famille de la tile
    char family[5];
    /// @brief Parties de la tile
    tile_part_type_t parts[9];
    /// @brief Spécifie si la tile est un blason
    bool blason;
    /// @brief Spécifie le nombre de tiles présentes
    unsigned int amount;
    /// @brief Définition des groupes a l'intérieur d'une tile
    tile_part_group_t parts_groups[9];
} tile_t;

typedef struct placed_tile
{
    tile_t* parent;
    meeple_t* meeple;
} placed_tile_t;

#endif
#ifndef H_LIBCARCASSONNE_DECK_COMPOSITION
#define H_LIBCARCASSONNE_DECK_COMPOSITION

#include <libcarcassonne/tile.h>

#define VILLAGE LIBCARCASSONNE_TILE_PART_VILLAGE
#define FIELD   LIBCARCASSONNE_TILE_PART_FIELD
#define ROAD    LIBCARCASSONNE_TILE_PART_ROAD
#define WALL    LIBCARCASSONNE_TILE_PART_WALL
#define ABBEY   LIBCARCASSONNE_TILE_PART_ABBEY
#define TOWN    LIBCARCASSONNE_TILE_PART_TOWN

#define A LIBCARCASSONNE_TILE_PART_A
#define B LIBCARCASSONNE_TILE_PART_B
#define C LIBCARCASSONNE_TILE_PART_C
#define D LIBCARCASSONNE_TILE_PART_D
#define E LIBCARCASSONNE_TILE_PART_E
#define F LIBCARCASSONNE_TILE_PART_F
#define G LIBCARCASSONNE_TILE_PART_G
#define H LIBCARCASSONNE_TILE_PART_H
#define I LIBCARCASSONNE_TILE_PART_I

static tile_t
    tiles[] =
        {
            // CCCC
            {.family = "CCCC",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                 },
             .parts_groups = {A, A, A, A, 0, A, A, A, A},
             .blason       = 1,
             .amount       = 1,
             .texture="tile_11.png"},

            // CCCF

            {.family = "CCFC",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, B, B, B},
             .blason       = 1,
             .amount       = 1,
             .texture="tile_13.png"},

            {.family = "CCFC",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, B, B, B},
             .blason       = 0,
             .amount       = 3,
             .texture="tile_12.png"},

            // CCCR

            {.family = "CCRC",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, B, C, D},
             .blason       = 1,
             .amount       = 2,
             .texture="tile_15.png"},

            {.family = "CCRC",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     TOWN,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, B, C, D},
             .blason       = 0,
             .amount       = 1,
             .texture="tile_14.png"},

            // CCFF

            {// deux villes séparées

             .family = "CCFF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     WALL,
                     FIELD,
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {C, A, 0, C, 0, B, C, C, C},
             .blason       = 0,
             .amount       = 2,
             .texture="tile_16.png"},

            {.family = "CCFF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     TOWN,
                     FIELD,
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, B, B, A, 0, B, A, A, A},
             .blason       = 1,
             .amount       = 2,
             .texture = "tile_02.png"},

            {.family = "CCFF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     WALL,
                     FIELD,
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, B, B, A, 0, B, A, A, A},
             .blason       = 0,
             .amount       = 3,
             .texture = "tile_01.png"},

            // CCRR
            {.family = "CCRR",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     FIELD,
                     TOWN,
                     FIELD,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, B, A, 0, C, B, C, D},
             .blason       = 1,
             .amount       = 2,
             .texture="tile_24.png"},

            {.family = "CCRR",
             .parts =
                 {
                     TOWN,
                     TOWN,
                     FIELD,
                     TOWN,
                     FIELD,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, B, A, 0, C, B, C, D},
             .blason       = 0,
             .amount       = 3,
             .texture="tile_23.png"},

            // CFCF

            {
                .family = "FCFC",
                .parts =
                    {
                        FIELD,
                        FIELD,
                        FIELD,
                        TOWN,
                        TOWN,
                        TOWN,
                        FIELD,
                        FIELD,
                        FIELD,
                    },
                .parts_groups = {A, A, A, B, 0, B, C, C, C},
                .blason       = 1,
                .amount       = 2,
                .texture="tile_22.png"},

            {.family = "FCFC",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     TOWN,
                     TOWN,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, B, 0, B, C, C, C},
             .blason       = 0,
             .amount       = 1,
             .texture="tile_21.png"},

            {.family = "CFCF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     TOWN,
                     FIELD,
                 },
             .parts_groups = {A, A, A, B, 0, B, A, A, A},
             .blason       = 0,
             .amount       = 3,
             .texture = "tile_04.png"},

            // CFFF

            {.family = "CFFF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, B, A, A, 0, A, A, A, A},
             .blason       = 0,
             .amount       = 5,
             .texture="tile_17.png"},

            // CFRR

            {.family = "CFRR",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     ROAD,
                     ROAD,
                     FIELD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, B, A, C, 0, A, D, C, A},
             .blason       = 0,
             .amount       = 3,
             .texture="tile_20.png"},

            // CRFR

            {.family = "CRFR",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     ROAD,
                     ROAD,
                     ROAD,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, D, A, B, 0, B, C, C, C},
             .blason       = 0,
             .amount       = 3,
             .texture="tile_18.png"},

            // CRRF
            {.family = "CRRF",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     FIELD,
                     ROAD,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, D, A, A, 0, B, A, B, C},
             .blason       = 0,
             .amount       = 3,
             .texture="tile_19.png"},

            // CRRR
            {.family = "CRRR",
             .parts =
                 {
                     FIELD,
                     TOWN,
                     FIELD,
                     ROAD,
                     VILLAGE,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {G, A, G, B, 0, C, F, D, E},
             .blason       = 0,
             .amount       = 3,
             .texture = "tile_03.png"},

            // FFFF
            {.family = "FFFF",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     ABBEY,
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, A, A, A},
             .blason       = 0,
             .amount       = 4,
             .texture = "tile_00.png"},

            // FFFR
            {.family = "FFFR",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     FIELD,
                     ABBEY,
                     FIELD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, A, 0, A, A, B, A},
             .blason       = 0,
             .amount       = 2,
             .texture ="tile_06.png"},

            // FFRR

            {.family = "FFRR",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     ROAD,
                     ROAD,
                     FIELD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, B, 0, A, C, B, A},
             .blason       = 0,
             .amount       = 9,
             .texture ="tile_08.png"},

            // FRFR
            {.family = "FRFR",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     ROAD,
                     ROAD,
                     ROAD,
                     FIELD,
                     FIELD,
                     FIELD,
                 },
             .parts_groups = {A, B, C, A, 0, C, A, B, C},
             .blason       = 0,
             .amount       = 8,
             .texture="tile_09.png"},

            // FRRR

            {.family = "FRRR",
             .parts =
                 {
                     FIELD,
                     FIELD,
                     FIELD,
                     ROAD,
                     VILLAGE,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, A, A, B, 0, C, E, D, F},
             .blason       = 0,
             .amount       = 4,
             .texture="tile_10.png"},

            // RRRR

            {.family = "RRRR",
             .parts =
                 {
                     FIELD,
                     ROAD,
                     FIELD,
                     ROAD,
                     VILLAGE,
                     ROAD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {E, A, F, B, 0, C, G, D, H},
             .blason       = 0,
             .amount       = 1,
             .texture = "tile_07.png"},
};

static tile_t start_tile = {.family = "CRFR",
                            .parts =
                                {
                                    FIELD,
                                    TOWN,
                                    FIELD,
                                    ROAD,
                                    ROAD,
                                    ROAD,
                                    FIELD,
                                    FIELD,
                                    FIELD,
                                },
                            .parts_groups = {D, A, D, B, 0, B, C, C, C},
                            .blason       = 0,
                            .amount       = 1,
                            .texture = "tile_18.png"};

// On supprime les macros qu'on a utilisé

#undef VILLAGE
#undef FIELD
#undef ROAD
#undef WALL
#undef ABBEY
#undef TOWN

#endif

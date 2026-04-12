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
             .amount       = 1},

            // CCCF

            {.family = "CCCF",
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
             .amount       = 1},

            {.family = "CCCF",
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
             .amount       = 3},

            // CCCR

            {.family = "CCCR",
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
             .amount       = 2},

            {.family = "CCCR",
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
             .amount       = 1},

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
             .amount       = 2},

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
             .amount       = 2},

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
             .amount       = 3},

            // CCRR
            {.family = "CCRR",
             .parts =
                 {
                     FIELD,TOWN,TOWN,
                     ROAD,FIELD,TOWN,
                     FIELD,ROAD,FIELD,
                 },
             .parts_groups = {
                A, B, B, 
                C, 0, B, 
                D, C, A
            },
             .blason       = 1,
             .amount       = 2},

            {.family = "CCRR",
             .parts =
                 {
                     FIELD,TOWN,TOWN,
                     ROAD,FIELD,TOWN,
                     FIELD,ROAD,FIELD,
                 },
             .parts_groups = {
                A, B, B, 
                C, 0, B, 
                D, C, A
            },
             .blason       = 0,
             .amount       = 3},

            // CFCF

            {
                .family = "CFCF",
                .parts =
                    {
                        FIELD, TOWN, FIELD,
                        FIELD, FIELD, FIELD,
                        FIELD, TOWN, FIELD
                    },
                .parts_groups = {
                    A, B, A, 
                    A, 0, A, 
                    A, B, A
                },
                .blason       = 0,
                .amount       = 3},

            {.family = "CFCF",
             .parts =
                 {
                     FIELD,TOWN,FIELD,
                     FIELD,TOWN,FIELD,
                     FIELD,TOWN,FIELD,
                 },
             .parts_groups = {
                A, B, C, 
                A, 0, C, 
                A, B, C
            },
             .blason       = 0,
             .amount       = 1},

            {.family = "CFCF",
             .parts =
                 {
                     FIELD,TOWN,FIELD,
                     FIELD,TOWN,FIELD,
                     FIELD,TOWN,FIELD,
                 },
             .parts_groups = {
                A, B, C, 
                A, 0, C, 
                A, B, C
            },
             .blason       = 1,
             .amount       = 2},

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
             .amount       = 5},

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
             .amount       = 3},

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
             .amount       = 3},

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
             .amount       = 3},

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
             .amount       = 3},

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
             .amount       = 4},

            // FFFR
            {.family = "FFFR",
             .parts =
                 {
                     FIELD,FIELD,FIELD,
                     ROAD,ABBEY,FIELD,
                     FIELD,FIELD,FIELD,
                 },
             .parts_groups = {
                A, A, A, 
                B, 0, A, 
                A, A, A
            },
             .blason       = 0,
             .amount       = 2},

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
             .amount       = 9},

            // FRFR
            {.family = "FRFR",
             .parts =
                 {
                     FIELD,
                     ROAD,
                     FIELD,
                     FIELD,
                     ROAD,
                     FIELD,
                     FIELD,
                     ROAD,
                     FIELD,
                 },
             .parts_groups = {A, B, C, A, 0, C, A, B, C},
             .blason       = 0,
             .amount       = 8},

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
             .amount       = 4},

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
             .amount       = 1},
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
                            .amount       = 1};

// On supprime les macros qu'on a utilisé

#undef VILLAGE
#undef FIELD
#undef ROAD
#undef WALL
#undef ABBEY
#undef TOWN

#endif

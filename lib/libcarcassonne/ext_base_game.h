#pragma once

#include <libcarcassonne/ext_base_game_hooks.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/tile.h>
#include <unistd.h>

#include "ext_base_game_hooks.h"
#include "libcarcassonne/consts.h"

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

static const tile_t LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[] =
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
         .blason       = true,
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
         .blason       = true,
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
         .blason       = false,
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
         .blason       = true,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = true,
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
         .blason       = false,
         .amount       = 3},

        // CCRR
        {.family = "CCRR",
         .parts =
             {
                 FIELD,
                 TOWN,
                 TOWN,
                 ROAD,
                 FIELD,
                 TOWN,
                 FIELD,
                 ROAD,
                 FIELD,
             },
         .parts_groups = {A, B, B, C, 0, B, D, C, A},
         .blason       = true,
         .amount       = 2},

        {.family = "CCRR",
         .parts =
             {
                 FIELD,
                 TOWN,
                 TOWN,
                 ROAD,
                 FIELD,
                 TOWN,
                 FIELD,
                 ROAD,
                 FIELD,
             },
         .parts_groups = {A, B, B, C, 0, B, D, C, A},
         .blason       = false,
         .amount       = 3},

        // CFCF

        {.family = "CFCF",
         .parts = {FIELD, TOWN, FIELD, FIELD, FIELD, FIELD, FIELD, TOWN, FIELD},
         .parts_groups = {A, C, A, A, 0, A, A, B, A},
         .blason       = false,
         .amount       = 3},

        {.family = "CFCF",
         .parts =
             {
                 FIELD,
                 TOWN,
                 FIELD,
                 FIELD,
                 TOWN,
                 FIELD,
                 FIELD,
                 TOWN,
                 FIELD,
             },
         .parts_groups = {A, B, C, A, 0, C, A, B, C},
         .blason       = false,
         .amount       = 1},

        {.family = "CFCF",
         .parts =
             {
                 FIELD,
                 TOWN,
                 FIELD,
                 FIELD,
                 TOWN,
                 FIELD,
                 FIELD,
                 TOWN,
                 FIELD,
             },
         .parts_groups = {A, B, C, A, 0, C, A, B, C},
         .blason       = true,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
         .amount       = 4},

        // FFFR
        {.family = "FFFR",
         .parts =
             {
                 FIELD,
                 FIELD,
                 FIELD,
                 ROAD,
                 ABBEY,
                 FIELD,
                 FIELD,
                 FIELD,
                 FIELD,
             },
         .parts_groups = {A, A, A, B, 0, A, A, A, A},
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
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
         .blason       = false,
         .amount       = 1,
         .slots = {
            {
                .x=0,
                .y=1,
                .group=A
            }
         },
         .nb_slots=1
        },
};

static const tile_vector_t LIBCARCASSONNE_EXT_BASE_GAME_TILES = {
    .meta = {.size = sizeof(LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS) /
                     sizeof(LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0]),
             .caps = sizeof(LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS) /
                     sizeof(LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0]),
             .data = (tile_t*)&LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[0]}};

const static tile_t LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS[] = {
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
     .parts_groups = {D, A, D, B, 0, B, C, C, C},
     .blason       = false,
     .amount       = 1,
     .texture      = "tile_18.png"}};

static const tile_vector_t LIBCARCASSONNE_EXT_BASE_GAME_START_TILES = {
    .meta = {
        .size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS),
        .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS),
        .data = (tile_t*)&LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS[0]}};

static const extension_process_hook_t*
    LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST[] = {
        &hook_meeple_place, &hook_tile_place, &hook_give_back_meeples,
        &hook_next_player};

static const extension_process_hooks_vector_t
    LIBCARCASSONNE_EXT_BASE_GAME_HOOKS = {
        .meta = {
            .size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST),
            .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST),
            .data = &LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST,
        }};

static const extension_vector_t LIBCARCASSONNE_EXT_BASE_GAME_REQUIREMENTS = {
    .meta = {.size = 0, .caps = 0, .data = NULL}};

static meeple_count_t LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT[] = {
    {.count = 5, .meeple_type = BASIC}};

static const meeple_count_vector_t meeples_count = {
    .meta = {.size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT),
             .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT),
             .data = &LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT}};

static const extension_t LIBCARCASSONNE_EXT_BASE_GAME = {
    .name                 = "Base Game",
    .hooks                = &LIBCARCASSONNE_EXT_BASE_GAME_HOOKS,
    .tiles                = &LIBCARCASSONNE_EXT_BASE_GAME_TILES,
    .start_tiles          = &LIBCARCASSONNE_EXT_BASE_GAME_START_TILES,
    .start_tiles_priority = 0,
    .required             = &LIBCARCASSONNE_EXT_BASE_GAME_REQUIREMENTS,
};

#undef VILLAGE
#undef FIELD
#undef ROAD
#undef WALL
#undef ABBEY
#undef TOWN

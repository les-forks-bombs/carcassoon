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
         .amount       = 1,
         .texture = "tile_11.svg",
         .slots = {
            {
                .x= 0.5,
                .y= 0.5,
                .group = A
            }
         },
         .nb_slots = 1
        },

        // CCCF

        {.family = "CCFC",
         .parts =
             {
                 WALL,
                 TOWN,
                 WALL,
                 TOWN,
                 TOWN,
                 TOWN,
                 FIELD,
                 FIELD,
                 FIELD,
             },
         .parts_groups = {A, A, A, A, 0, A, B, B, B},
         .blason       = true,
         .amount       = 1,
        .texture = "tile_13.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.35,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.9,
                .group = B
            }
        },
        .nb_slots = 2
    },

        {.family = "CCFC",
         .parts =
             {
                 WALL,
                 TOWN,
                 WALL,
                 TOWN,
                 TOWN,
                 TOWN,
                 FIELD,
                 FIELD,
                 FIELD,
             },
         .parts_groups = {A, A, A, A, 0, A, B, B, B},
         .blason       = false,
         .amount       = 3,
        .texture = "tile_12.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.35,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.9,
                .group = B
            }
        },
        .nb_slots = 2
    },

        // CCCR

        {.family = "CCRC",
         .parts =
             {
                 WALL,
                 TOWN,
                 WALL,
                 TOWN,
                 TOWN,
                 TOWN,
                 FIELD,
                 ROAD,
                 FIELD,
             },
         .parts_groups = {A, A, A, A, 0, A, B, C, D},
         .blason       = true,
         .amount       = 2,
        .texture = "tile_15.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.35,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.9,
                .group = C
            },
            {
                .x= 0.28,
                .y= 0.92,
                .group = B
            },
            {
                .x= 0.72,
                .y= 0.92,
                .group = D
            }
        },
        .nb_slots = 4
    },

        {.family = "CCRC",
         .parts =
             {
                 WALL,
                 TOWN,
                 WALL,
                 TOWN,
                 TOWN,
                 TOWN,
                 FIELD,
                 ROAD,
                 FIELD,
             },
         .parts_groups = {A, A, A, A, 0, A, B, C, D},
         .blason       = false,
         .amount       = 1,
        .texture = "tile_14.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.35,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.9,
                .group = C
            },
            {
                .x= 0.28,
                .y= 0.92,
                .group = B
            },
            {
                .x= 0.72,
                .y= 0.92,
                .group = D
            }
        },
        .nb_slots = 4
    },

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
         .amount       = 2,
         .texture = "tile_16.svg",
         .slots = {
            {
                .x= 0.5,
                .y= 0.1,
                .group = A
            },
            {
                .x= 0.3,
                .y= 0.7,
                .group = C
            },
            {
                .x= 0.9,
                .y= 0.5,
                .group = B
            }
         },
         .nb_slots = 3
    },

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
         .blason       = true,
         .amount       = 2,
         .texture = "tile_02.svg",
        .slots = {
            {
                .x = 0.77,
                .y = 0.23,
                .group = B
            },
            {
                .x = 0.3,
                .y = 0.7,
                .group = A
            }
        },
        .nb_slots = 2
        },

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
         .amount       = 3,
        .texture = "tile_01.svg",
        .slots = {
            {
                .x = 0.77,
                .y = 0.23,
                .group = B
            },
            {
                .x = 0.3,
                .y = 0.7,
                .group = A
            }
        },
        .nb_slots = 2
    },

        // CCRR
        {.family = "CCRR",
         .parts =
             {
                 FIELD,
                 TOWN,
                 WALL,
                 ROAD,
                 FIELD,
                 TOWN,
                 FIELD,
                 ROAD,
                 FIELD,
             },
         .parts_groups = {A, B, B, C, 0, B, D, C, A},
         .blason       = true,
         .amount       = 2,
        .texture = "tile_24.svg"
    },

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
         .amount       = 3,
        .texture = "tile_23.svg"
    },

        // CFCF

        {.family = "CFCF",
         .parts = {FIELD, TOWN, FIELD, FIELD, FIELD, FIELD, FIELD, TOWN, FIELD},
         .parts_groups = {A, C, A, A, 0, A, A, B, A},
         .blason       = false,
         .amount       = 3,
         .texture = "tile_04.svg",
         .slots = {
            {
                .x = 0.5,
                .y = 0.1,
                .group = C
            },
            {
                .x = 0.5,
                .y = 0.9,
                .group = B
            },
            {
                .x = 0.5,
                .y = 0.5,
                .group = A
            }
         },
         .nb_slots = 3
         },

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
         .parts_groups = {A, B, C, A, 0, C, A, B, C},
         .blason       = false,
         .amount       = 1,
        .texture = "tile_21.svg"
    },

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
         .parts_groups = {A, B, C, A, 0, C, A, B, C},
         .blason       = true,
         .amount       = 2,
        .texture = "tile_22.svg"
    },

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
         .amount       = 5,
        .texture = "tile_17.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.1,
                .group = B
            },
            {
                .x= 0.5,
                .y= 0.65,
                .group = A
            }
        },
        .nb_slots=2
    },

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
         .amount       = 3,
        .texture = "tile_20.svg"
    },

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
         .amount       = 3,
        .texture = "tile_18.svg",
        .slots = {
            {
                .x= 0.5,
                .y= 0.1,
                .group = D
            },
            {
                .x= 0.2,
                .y= 0.3,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.5,
                .group = B
            },
            {
                .x= 0.5,
                .y= 0.79,
                .group = C
            }
        },
        .nb_slots = 4
    },

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
         .amount       = 3,
        .texture = "tile_19.svg",
        .slots = {
            {
            .x= 0.5,
            .y= 0.1,
            .group = D
            },
            {
            .x= 0.25,
            .y= 0.5,
            .group = A
            },
            {
            .x= 0.65,
            .y= 0.65,
            .group = B
            },
            {
            .x= 0.83,
            .y= 0.83,
            .group = C
            },
        },
        .nb_slots = 4
    },

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
         .amount       = 3,
         .texture = "tile_03.svg",
         .slots = {
                        {
                .x = 0.8,
                .y = 0.8,
                .group = E
            },
            {
                .x = 0.2,
                .y = 0.8,
                .group = F
            },
            {
                .x = 0.5,
                .y = 0.1,
                .group = A
            },
            {
                .x = 0.2,
                .y = 0.3,
                .group = G
            },
            {
                .x = 0.18,
                .y = 0.5,
                .group = B
            },
            {
                .x = 0.82,
                .y = 0.5,
                .group = C
            },
            {
                .x = 0.5,
                .y = 0.82,
                .group = D
            }
         },
         .nb_slots = 7
        },

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
         .amount       = 4,
        .texture = "tile_00.svg",
        .slots = {
            {
                .x = 0.5,
                .y = 0.5,
                .group = 0
            },
            {
                .x = 0.8,
                .y = 0.8,
                .group = A
            },
         },
         .nb_slots = 2
    },

        // FFFR
        {.family = "FFRF",
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
         .parts_groups = {A, A, A, B, 0, A, A, A, A},
         .blason       = false,
         .amount       = 2,
         .texture = "tile_06.svg",
        .slots = {
            {
                .x = 0.5,
                .y = 0.5,
                .group = 0
            },
            {
                .x = 0.2,
                .y = 0.2,
                .group = A
            },
            {
                .x = 0.5,
                .y = 0.82,
                .group = B
            }
         },
         .nb_slots = 3
        },

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
         .amount       = 9,
         .texture = "tile_08.svg",
         .slots = {
            {
                .x= 0.35,
                .y= 0.65,
                .group = B
            },
            {
                .x= 0.7,
                .y= 0.3,
                .group = A
            },
            {
                .x = 0.17,
                .y = 0.83,
                .group = C
            }
         },
         .nb_slots=3
        },

        // RFRF
        {.family = "RFRF",
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
         .parts_groups = {A, A, A, B, 0, B, C, C, C},
         .blason       = false,
         .amount       = 8,
         .texture = "tile_09.svg",
         .slots = {
            {
                .x= 0.5,
                .y= 0.5,
                .group = B
            },
            {
                .x= 0.5,
                .y= 0.21,
                .group = A
            },
            {
                .x= 0.5,
                .y= 0.79,
                .group = C
            }
         },
         .nb_slots=3
        },

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
         .amount       = 4,
         .texture = "tile_10.svg",
         .slots = {
            {
                .x = 0.8,
                .y = 0.8,
                .group = F
            },
            {
                .x = 0.2,
                .y = 0.8,
                .group = E
            },
            {
                .x = 0.5,
                .y = 0.21,
                .group = A
            },
            {
                .x = 0.18,
                .y = 0.5,
                .group = B
            },
            {
                .x = 0.82,
                .y = 0.5,
                .group = C
            },
            {
                .x = 0.5,
                .y = 0.82,
                .group = D
            }
         },
         .nb_slots = 6
        },

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
         .texture = "tile_07.svg",
         .slots = {
            {
                .x = 0.5,
                .y = 0.18,
                .group = A
            },
            {
                .x= 0.2,
                .y= 0.2,
                .group = E
            },
            {
                .x= 0.8,
                .y= 0.2,
                .group = F
            },
            {
                .x= 0.18,
                .y= 0.5,
                .group = B
            },
            {
                .x= 0.82,
                .y= 0.5,
                .group = C
            },
            {
                .x= 0.2,
                .y= 0.8,
                .group = G
            },
            {
                .x= 0.5,
                .y= 0.82,
                .group = D
            },
            {
                .x= 0.8,
                .y= 0.8,
                .group = H
            },
         },
         .nb_slots=8
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
     .texture      = "tile_18.png",
    .slots = {
    {
            .x= 0.5,
            .y= 0.1,
            .group = A
        },
    {
            .x= 0.2,
            .y= 0.3,
            .group = D
        },
    {
            .x= 0.5,
            .y= 0.5,
            .group = B
        },
    {
            .x= 0.5,
            .y= 0.79,
            .group = C
        },
    },
    .nb_slots=4
}};

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
    .meeples_count        = meeples_count};

#undef VILLAGE
#undef FIELD
#undef ROAD
#undef WALL
#undef ABBEY
#undef TOWN

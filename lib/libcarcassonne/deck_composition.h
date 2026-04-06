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


static tile_t tiles[] = {
    // CCCC
    {
        .family = "CCCC",
        .parts = {
            TOWN, TOWN, TOWN,
            TOWN, TOWN, TOWN,
            TOWN, TOWN, TOWN,
        },
        .parts_groups = {
            A, A, A,
            A, A, A,
            A, A, A
        },
        .blason = 1,
        .amount = 1
    },

    // CCCF

    {
        .family = "CCCF",
        .parts = {
            TOWN , TOWN , TOWN ,
            TOWN , TOWN , TOWN ,
            FIELD, FIELD, FIELD,
        },
        .parts_groups = {
            A, A, A,
            A, A, A,
            B, B, B
        },
        .blason = 1,
        .amount = 1
    },

    {
        .family = "CCCF",
        .parts = {
            TOWN , TOWN , TOWN ,
            TOWN , TOWN , TOWN ,
            FIELD, FIELD, FIELD,
        },
        .parts_groups = {
            A, A, A,
            A, A, A,
            B, B, B
        },
        .blason = 0,
        .amount = 3
    },

    // CCCR

    {
        .family = "CCCR",
        .parts = {
            TOWN , TOWN , TOWN ,
            TOWN , TOWN , TOWN ,
            FIELD, ROAD , FIELD,
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CCCR",
        .parts = {
            TOWN , TOWN , TOWN ,
            TOWN , TOWN , TOWN ,
            FIELD, ROAD , FIELD,
        },
        .parts_groups = {
            A, A, A,
            A, A, A,
            B, B, B
        },
        .blason = 0,
        .amount = 1
    },

    // CCFF

    {
        // deux viles séparées

        .family = "CCFF",
        .parts = {
            TOWN , TOWN , WALL ,
            FIELD, FIELD, TOWN ,
            FIELD, FIELD, WALL,
        },
        .parts_groups = {
            A, A, A,
            A, A, A,
            B, B, B
        },
        .blason = 0,
        .amount = 2
    },
    
    {
        .family = "CCFF",
        .parts = {
            TOWN , TOWN , TOWN,
            FIELD, TOWN,  TOWN,
            FIELD, FIELD, TOWN,
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CCFF",
        .parts = {
            TOWN , TOWN , TOWN,
            FIELD, TOWN,  TOWN,
            FIELD, FIELD, TOWN,
        },
        .blason = 0,
        .amount = 3
    },

    // CCRR
    {
        .family = "CCRR",
        .parts = {
            TOWN , TOWN , TOWN,
            TOWN , TOWN,  ROAD ,
            ROAD , ROAD , ROAD ,
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CCRR",
        .parts = {
            TOWN , TOWN , TOWN,
            TOWN , TOWN,  ROAD ,
            ROAD , ROAD , ROAD ,
        },
        .blason = 0,
        .amount = 3
    },

    // CFCF

    {
        .family = "CFCF",
        .parts = {
            TOWN , FIELD, TOWN,
            TOWN , TOWN,  TOWN,
            TOWN,  FIELD, TOWN,
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CFCF",
        .parts = {
            TOWN , FIELD, TOWN,
            TOWN , TOWN,  TOWN,
            TOWN,  FIELD, TOWN,
        },
        .blason = 0,
        .amount = 1
    },

    {
        .family = "CFCF",
        .parts = {
            TOWN , FIELD, TOWN,
            TOWN , FIELD, TOWN,
            TOWN,  FIELD, TOWN,
        },
        .blason = 0,
        .amount = 3
    },

    // CFFF

    {
        .family = "CFFF",
        .parts = {
            TOWN  , TOWN , TOWN,
            FIELD , FIELD, FIELD,
            FIELD , FIELD, FIELD,
        },
        .blason = 0,
        .amount = 5
    },

    // CFRR

    {
        .family = "CFRR",
        .parts = {
            TOWN  , TOWN , TOWN,
            ROAD  , ROAD , FIELD,
            FIELD , ROAD , FIELD,
        },
        .blason = 0,
        .amount = 3
    },

    // CRFR

    {
        .family = "CRFR",
        .parts = {
            TOWN  , TOWN , TOWN,
            ROAD  , ROAD , ROAD ,
            FIELD , FIELD, FIELD,
        },
        .blason = 0,
        .amount = 3
    },

    // CRRF
    {
        .family = "CRRF",
        .parts = {
            TOWN  , TOWN , TOWN,
            FIELD , ROAD , ROAD ,
            FIELD , ROAD , FIELD,
        },
        .blason = 0,
        .amount = 3
    },

    // CRRR
    {
        .family = "CRRR",
        .parts = {
            TOWN  , TOWN   , TOWN,
            ROAD  , VILLAGE, ROAD ,
            FIELD , ROAD   , FIELD,
        },
        .blason = 0,
        .amount = 3
    },

    // FFFF
    {
        .family = "FFFF",
        .parts = {
            FIELD , FIELD , FIELD,
            FIELD , ABBEY , FIELD,
            FIELD , FIELD , FIELD,
        },
        .blason = 0,
        .amount = 4
    },

    // FFFR
    {
        .family = "FFFR",
        .parts = {
            FIELD , FIELD , FIELD,
            FIELD , ABBEY , FIELD,
            FIELD , ROAD  , FIELD,
        },
        .blason = 0,
        .amount = 2
    },

    // FFRR
    
    {
        .family = "FFRR",
        .parts = {
            FIELD , FIELD , FIELD,
            ROAD  , ROAD  , FIELD,
            FIELD , ROAD  , FIELD,
        },
        .blason = 0,
        .amount = 9
    },

    // FRFR
    {
        .family = "FRFR",
        .parts = {
            FIELD , ROAD  , FIELD,
            FIELD , ROAD  , FIELD,
            FIELD , ROAD  , FIELD,
        },
        .blason = 0,
        .amount = 8
    },

    // FRRR

    {
        .family = "FRRR",
        .parts = {
            FIELD , FIELD  , FIELD,
            ROAD  , VILLAGE, ROAD ,
            FIELD , ROAD   , FIELD,
        },
        .blason = 0,
        .amount = 4
    },

    // RRRR

    {
        .family = "RRRR",
        .parts = {
            FIELD , ROAD   , FIELD,
            ROAD  , VILLAGE, ROAD ,
            FIELD , ROAD   , FIELD,
        },
        .blason = 0,
        .amount = 1
    },
};

static tile_t start_tile = {
    .family = "CRFR",
    .parts = {
        TOWN  , TOWN , TOWN,
        ROAD  , ROAD , ROAD ,
        FIELD , FIELD, FIELD,
    },
    .blason = 0,
    .amount = 1
};

// On supprime les macros qu'on a utilisé

#undef VILLAGE
#undef FIELD  
#undef ROAD   
#undef WALL   
#undef ABBEY  
#undef TOWN   

#endif

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
            A,    A,
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
            A,    A,
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
            A,    A,
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
        .parts_groups = {
            A, A, A,
            A,    A,
            B, C, D
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
            A,    A,
            B, C, D
        },
        .blason = 0,
        .amount = 1
    },

    // CCFF

    {
        // deux villes séparées

        .family = "CCFF",
        .parts = {
            FIELD , TOWN , WALL ,
            FIELD, FIELD, TOWN ,
            FIELD, FIELD, FIELD,
        },
        .parts_groups = {
            C, A, 0,
            C,    B,
            C, C, C
        },
        .blason = 0,
        .amount = 2
    },
    
    {
        .family = "CCFF",
        .parts = {
            FIELD , TOWN , TOWN,
            FIELD, FIELD, TOWN ,
            FIELD, FIELD, FIELD,
        },
        .parts_groups = {
            A, B, B,
            A,    B,
            A, A, A
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CCFF",
        .parts = {
            FIELD , TOWN , WALL ,
            FIELD, FIELD, TOWN ,
            FIELD, FIELD, FIELD,
        },
        .parts_groups = {
            A, B, B,
            A,    B,
            A, A, A
        },
        .blason = 0,
        .amount = 3
    },

    // CCRR
    {
        .family = "CCRR",
        .parts = {
            TOWN , TOWN , FIELD,
            TOWN , FIELD,  ROAD ,
            FIELD , ROAD , FIELD ,
        },
        .parts_groups = {
            A, A, B,
            A,    C,
            B, C, D
        },
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CCRR",
        .parts = {
            TOWN , TOWN , FIELD,
            TOWN , FIELD,  ROAD ,
            FIELD , ROAD , FIELD ,
        },
        .parts_groups = {
            A, A, B,
            A,    C,
            B, C, D
        },
        .blason = 0,
        .amount = 3
    },

    // CFCF

    {
        .family = "CFCF",
        .parts = {
            FIELD , FIELD, FIELD,
            TOWN , TOWN,  TOWN,
            FIELD,  FIELD, FIELD,
        },
        .parts_groups = {
            A, A, A,
            B,    B,
            C, C, C
        },  
        .blason = 1,
        .amount = 2
    },

    {
        .family = "CFCF",
        .parts = {
            FIELD , FIELD, FIELD,
            TOWN , TOWN,  TOWN,
            FIELD,  FIELD, FIELD,
        },
        .parts_groups = {
            A, A, A,
            B,    B,
            C, C, C
        },  
        .blason = 0,
        .amount = 1
    },

    {
        .family = "CFCF",
        .parts = {
            FIELD , FIELD, FIELD,
            TOWN , FIELD,  TOWN,
            FIELD,  FIELD, FIELD,
        },
        .parts_groups = {
            A, A, A,
            B,    B,
            A, A, A
        },  
        .blason = 0,
        .amount = 3
    },

    // CFFF

    {
        .family = "CFFF",
        .parts = {
            FIELD  , TOWN , FIELD,
            FIELD , FIELD, FIELD,
            FIELD , FIELD, FIELD,
        },
        .parts_groups = {
            A, B, A,
            A,    A,
            A, A, A
        },  
        .blason = 0,
        .amount = 5
    },

    // CFRR

    {
        .family = "CFRR",
        .parts = {
            FIELD  , TOWN , FIELD,
            ROAD , ROAD, FIELD,
            FIELD , ROAD, FIELD,
        },
        .parts_groups = {
            A, B, A,
            C,    A,
            D, C, A
        },  
        .blason = 0,
        .amount = 3
    },

    // CRFR

    {
        .family = "CRFR",
        .parts = {
            FIELD  , TOWN , FIELD,
            ROAD  , ROAD , ROAD ,
            FIELD , FIELD, FIELD,
        },
        .parts_groups = {
            A, D, A,
            B,    B,
            C, C, C
        },  
        .blason = 0,
        .amount = 3
    },

    // CRRF
    {
        .family = "CRRF",
        .parts = {
            FIELD  , TOWN , FIELD,
            FIELD , ROAD , ROAD ,
            FIELD , ROAD , FIELD,
        },
        .parts_groups = {
            A, D, A,
            A,    B,
            A, B, C
        },  
        .blason = 0,
        .amount = 3
    },

    // CRRR
    {
        .family = "CRRR",
        .parts = {
            FIELD  , TOWN   , FIELD,
            ROAD  , VILLAGE, ROAD ,
            FIELD , ROAD   , FIELD,
        },
        .parts_groups = {
            G, A, G,
            B,    C,
            F, D, E
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
        .parts_groups = {
            A, A, A,
            A,    A,
            A, A, A
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
        .parts_groups = {
            A, A, A,
            A,    A,
            A, B, A
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
        .parts_groups = {
            A, A, A,
            B,    A,
            C, B, A
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
        .parts_groups = {
            A, B, C,
            A,    C,
            A, B, C
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
        .parts_groups = {
            A, A, A,
            B,    C,
            E, D, F
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
        .parts_groups = {
            E, A, F,
            B,    C,
            G, D, H
        },  
        .blason = 0,
        .amount = 1
    },
};

static tile_t start_tile = {
    .family = "CRFR",
    .parts = {
        FIELD  , TOWN , FIELD,
        ROAD  , ROAD , ROAD ,
        FIELD , FIELD, FIELD,
    },
    .parts_groups = {
            D, A, D,
            B,    B,
            C, C, C
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

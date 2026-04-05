#ifndef H_LIBCARCASSONNE_DECK_COMPOSITION
#define H_LIBCARCASSONNE_DECK_COMPOSITION

#include <libcarcassonne/deck.h>

#define VILLAGE LIBCARCASSONNE_TILE_PART_VILLAGE
#define FIELD   LIBCARCASSONNE_TILE_PART_FIELD
#define ROAD    LIBCARCASSONNE_TILE_PART_ROAD 
#define WALL    LIBCARCASSONNE_TILE_PART_WALL
#define ABBEY   LIBCARCASSONNE_TILE_PART_ABBEY
#define TOWN    LIBCARCASSONNE_TILE_PART_TOWN

typedef struct deck_tile_prototype
{
    tile_t tile;
    int remaining;
} deck_tile_prototype_t;

static deck_tile_prototype_t tiles[] = {

    // CCCC
    {
        .tile = {
            .name = "CCCC",
            .parts = {
                TOWN, TOWN, TOWN,
                TOWN, TOWN, TOWN,
                TOWN, TOWN, TOWN,
            },
            .blason = 1
        },
        .remaining = 1
    },

    // CCCF

    {
        .tile = {
            .name = "CCCF",
            .parts = {
                TOWN , TOWN , TOWN ,
                TOWN , TOWN , TOWN ,
                FIELD, FIELD, FIELD,
            },
            .blason = 1
        },
        .remaining = 1
    },

    {
        .tile = {
            .name = "CCCF",
            .parts = {
                TOWN , TOWN , TOWN ,
                TOWN , TOWN , TOWN ,
                FIELD, FIELD, FIELD,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CCCR

    {
        .tile = {
            .name = "CCCR",
            .parts = {
                TOWN , TOWN , TOWN ,
                TOWN , TOWN , TOWN ,
                FIELD, ROAD , FIELD,
            },
            .blason = 1
        },
        .remaining = 2
    },

    {
        .tile = {
            .name = "CCCR",
            .parts = {
                TOWN , TOWN , TOWN ,
                TOWN , TOWN , TOWN ,
                FIELD, ROAD , FIELD,
            },
            .blason = 0
        },
        .remaining = 1
    },

    // CCFF

    {
        // deux viles séparées
        .tile = {
            .name = "CCFF",
            .parts = {
                TOWN , TOWN , WALL ,
                FIELD, FIELD, TOWN ,
                FIELD, FIELD, WALL,
            },
            .blason = 0
        },
        .remaining = 2
    },
    
    {
        .tile = {
            .name = "CCFF",
            .parts = {
                TOWN , TOWN , TOWN,
                FIELD, TOWN,  TOWN,
                FIELD, FIELD, TOWN,
            },
            .blason = 1
        },
        .remaining = 2
    },

    {
        .tile = {
            .name = "CCFF",
            .parts = {
                TOWN , TOWN , TOWN,
                FIELD, TOWN,  TOWN,
                FIELD, FIELD, TOWN,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CCRR
    {
        .tile = {
            .name = "CCRR",
            .parts = {
                TOWN , TOWN , TOWN,
                TOWN , TOWN,  ROAD ,
                ROAD , ROAD , ROAD ,
            },
            .blason = 1
        },
        .remaining = 2
    },

    {
        .tile = {
            .name = "CCRR",
            .parts = {
                TOWN , TOWN , TOWN,
                TOWN , TOWN,  ROAD ,
                ROAD , ROAD , ROAD ,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CFCF

    {
        .tile = {
            .name = "CFCF",
            .parts = {
                TOWN , FIELD, TOWN,
                TOWN , TOWN,  TOWN,
                TOWN,  FIELD, TOWN,
            },
            .blason = 1
        },
        .remaining = 2
    },

    {
        .tile = {
            .name = "CFCF",
            .parts = {
                TOWN , FIELD, TOWN,
                TOWN , TOWN,  TOWN,
                TOWN,  FIELD, TOWN,
            },
            .blason = 0
        },
        .remaining = 1
    },

    {
        .tile = {
            .name = "CFCF",
            .parts = {
                TOWN , FIELD, TOWN,
                TOWN , FIELD, TOWN,
                TOWN,  FIELD, TOWN,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CFFF

    {
        .tile = {
            .name = "CFFF",
            .parts = {
                TOWN  , TOWN , TOWN,
                FIELD , FIELD, FIELD,
                FIELD , FIELD, FIELD,
            },
            .blason = 0
        },
        .remaining = 5
    },

    // CFRR

    {
        .tile = {
            .name = "CFRR",
            .parts = {
                TOWN  , TOWN , TOWN,
                ROAD  , ROAD , FIELD,
                FIELD , ROAD , FIELD,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CRFR

    {
        .tile = {
            .name = "CRFR",
            .parts = {
                TOWN  , TOWN , TOWN,
                ROAD  , ROAD , ROAD ,
                FIELD , FIELD, FIELD,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CRRF
    {
        .tile = {
            .name = "CRRF",
            .parts = {
                TOWN  , TOWN , TOWN,
                FIELD , ROAD , ROAD ,
                FIELD , ROAD , FIELD,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // CRRR
    {
        .tile = {
            .name = "CRRR",
            .parts = {
                TOWN  , TOWN   , TOWN,
                ROAD  , VILLAGE, ROAD ,
                FIELD , ROAD   , FIELD,
            },
            .blason = 0
        },
        .remaining = 3
    },

    // FFFF
    {
        .tile = {
            .name = "FFFF",
            .parts = {
                FIELD , FIELD , FIELD,
                FIELD , ABBEY , FIELD,
                FIELD , FIELD , FIELD,
            },
            .blason = 0
        },
        .remaining = 4
    },

    // FFFR
    {
        .tile = {
            .name = "FFFR",
            .parts = {
                FIELD , FIELD , FIELD,
                FIELD , ABBEY , FIELD,
                FIELD , ROAD  , FIELD,
            },
            .blason = 0
        },
        .remaining = 2
    },

    // FFRR
    
    {
        .tile = {
            .name = "FFRR",
            .parts = {
                FIELD , FIELD , FIELD,
                ROAD  , ROAD  , FIELD,
                FIELD , ROAD  , FIELD,
            },
            .blason = 0
        },
        .remaining = 9
    },

    // FRFR
    {
        .tile = {
            .name = "FRFR",
            .parts = {
                FIELD , ROAD  , FIELD,
                FIELD , ROAD  , FIELD,
                FIELD , ROAD  , FIELD,
            },
            .blason = 0
        },
        .remaining = 8
    },

    // FRRR

    {
        .tile = {
            .name = "FRRR",
            .parts = {
                FIELD , FIELD  , FIELD,
                ROAD  , VILLAGE, ROAD ,
                FIELD , ROAD   , FIELD,
            },
            .blason = 0
        },
        .remaining = 4
    },

    // RRRR

    {
        .tile = {
            .name = "RRRR",
            .parts = {
                FIELD , ROAD   , FIELD,
                ROAD  , VILLAGE, ROAD ,
                FIELD , ROAD   , FIELD,
            },
            .blason = 0
        },
        .remaining = 1
    },
};

static deck_tile_prototype_t start_tile = {
    .tile = {
        .name = "CRFR",
        .parts = {
            TOWN  , TOWN , TOWN,
            ROAD  , ROAD , ROAD ,
            FIELD , FIELD, FIELD,
        },
        .blason = 0
    },
    .remaining = 1
};

// On supprime les macros qu'on a utilisé

#undef VILLAGE
#undef FIELD  
#undef ROAD   
#undef WALL   
#undef ABBEY  
#undef TOWN   

#endif
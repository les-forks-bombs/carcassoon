#ifndef H_LIBCARCASSONNE_DECK_COMPOSITION
#define H_LIBCARCASSONNE_DECK_COMPOSITION

#include <libcarcassonne/deck.h>

#define VILLAGE LIBCARCASSONNE_TILE_PART_VILLAGE
#define CHAMP   LIBCARCASSONNE_TILE_PART_CHAMP
#define ROUTE   LIBCARCASSONNE_TILE_PART_ROUTE
#define MURS    LIBCARCASSONNE_TILE_PART_MURS
#define ABBEY   LIBCARCASSONNE_TILE_PART_ABBEY
#define TOWN    LIBCARCASSONNE_TILE_PART_TOWN

static deck_tile_t tiles[] = {

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
                CHAMP, CHAMP, CHAMP,
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
                CHAMP, CHAMP, CHAMP,
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
                CHAMP, ROUTE, CHAMP,
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
                CHAMP, ROUTE, CHAMP,
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
                TOWN , TOWN , MURS ,
                CHAMP, CHAMP, TOWN ,
                CHAMP, CHAMP, MURS,
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
                CHAMP, TOWN,  TOWN,
                CHAMP, CHAMP, TOWN,
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
                CHAMP, TOWN,  TOWN,
                CHAMP, CHAMP, TOWN,
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
                TOWN , TOWN,  ROUTE,
                ROUTE, ROUTE, ROUTE,
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
                TOWN , TOWN,  ROUTE,
                ROUTE, ROUTE, ROUTE,
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
                TOWN , CHAMP, TOWN,
                TOWN , TOWN,  TOWN,
                TOWN,  CHAMP, TOWN,
            },
            .blason = 1
        },
        .remaining = 2
    },

    {
        .tile = {
            .name = "CFCF",
            .parts = {
                TOWN , CHAMP, TOWN,
                TOWN , TOWN,  TOWN,
                TOWN,  CHAMP, TOWN,
            },
            .blason = 0
        },
        .remaining = 1
    },

    {
        .tile = {
            .name = "CFCF",
            .parts = {
                TOWN , CHAMP, TOWN,
                TOWN , CHAMP, TOWN,
                TOWN,  CHAMP, TOWN,
            },
            .blason = 0
        },
        .remaining = 1
    },

    // CFFF

    {
        .tile = {
            .name = "CFFF",
            .parts = {
                TOWN  , TOWN , TOWN,
                CHAMP , CHAMP, CHAMP,
                CHAMP , CHAMP, CHAMP,
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
                ROUTE , ROUTE, CHAMP,
                CHAMP , ROUTE, CHAMP,
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
                ROUTE , ROUTE, ROUTE,
                CHAMP , CHAMP, CHAMP,
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
                CHAMP , ROUTE, ROUTE,
                CHAMP , ROUTE, CHAMP,
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
                ROUTE , VILLAGE, ROUTE,
                CHAMP , ROUTE  , CHAMP,
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
                CHAMP , CHAMP , CHAMP,
                CHAMP , ABBEY , CHAMP,
                CHAMP , CHAMP , CHAMP,
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
                CHAMP , CHAMP , CHAMP,
                CHAMP , ABBEY , CHAMP,
                CHAMP , ROUTE , CHAMP,
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
                CHAMP , CHAMP , CHAMP,
                ROUTE , ROUTE , CHAMP,
                CHAMP , ROUTE , CHAMP,
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
                CHAMP , ROUTE , CHAMP,
                CHAMP , ROUTE , CHAMP,
                CHAMP , ROUTE , CHAMP,
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
                CHAMP , CHAMP  , CHAMP,
                ROUTE , VILLAGE, ROUTE,
                CHAMP , ROUTE  , CHAMP,
            },
            .blason = 0
        },
        .remaining = 4
    },

    // RRRR

    {
        .tile = {
            .name = "FRRR",
            .parts = {
                CHAMP , ROUTE  , CHAMP,
                ROUTE , VILLAGE, ROUTE,
                CHAMP , ROUTE  , CHAMP,
            },
            .blason = 0
        },
        .remaining = 1
    },
};

static deck_tile_t start_tiles[] = {
    {
        .tile = {
            .name = "CRFR",
            .parts = {
                TOWN  , TOWN , TOWN,
                ROUTE , ROUTE, ROUTE,
                CHAMP , CHAMP, CHAMP,
            },
            .blason = 0
        },
        .remaining = 1
    },
};

#endif H_LIBCARCASSONNE_DECK_COMPOSITION
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/meeple.h>
#include <libcarcassonne/tile.h>
#include <unistd.h>

#include "ext_base_game_hooks.h"

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

/// @brief Ensemble des tuiles présentes dans le jeu de base
const tile_t
    LIBCARCASSONNE_EXT_BASE_GAME_TILES_ITEMS[] =
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
             .parts_groups = {A, A, A, A, B, A, A, A, A},
             .blason       = true,
             .amount       = 1,
             .texture      = "tile_11.png",
             .slots        = {{.x = 0.5, .y = 0.5, .group = A}},
             .nb_slots     = 1},

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
             .parts_groups = {A, A, A, A, C, A, B, B, B},
             .blason       = true,
             .amount       = 1,
             .texture      = "tile_13.png",
             .slots        = {{.x = 0.5, .y = 0.35, .group = A},
                              {.x = 0.5, .y = 0.9, .group = B}},
             .nb_slots     = 2},

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
             .parts_groups = {A, A, A, A, C, A, B, B, B},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_12.png",
             .slots        = {{.x = 0.5, .y = 0.35, .group = A},
                              {.x = 0.5, .y = 0.9, .group = B}},
             .nb_slots     = 2},

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
             .parts_groups = {A, A, A, A, E, A, B, C, D},
             .blason       = true,
             .amount       = 2,
             .texture      = "tile_15.png",
             .slots        = {{.x = 0.5, .y = 0.35, .group = A},
                              {.x = 0.5, .y = 0.9, .group = C},
                              {.x = 0.28, .y = 0.92, .group = B},
                              {.x = 0.72, .y = 0.92, .group = D}},
             .nb_slots     = 4},

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
             .parts_groups = {A, A, A, A, E, A, B, C, D},
             .blason       = false,
             .amount       = 1,
             .texture      = "tile_14.png",
             .slots        = {{.x = 0.5, .y = 0.35, .group = A},
                              {.x = 0.5, .y = 0.9, .group = C},
                              {.x = 0.28, .y = 0.92, .group = B},
                              {.x = 0.72, .y = 0.92, .group = D}},
             .nb_slots     = 4},

            // CCFF

            {
                // deux villes séparées

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
                .parts_groups = {C, A, D, C, E, B, C, C, C},
                .blason       = false,
                .amount       = 2,
                .texture      = "tile_16.png",
                .slots        = {{.x = 0.5, .y = 0.1, .group = A},
                                 {.x = 0.3, .y = 0.7, .group = C},
                                 {.x = 0.9, .y = 0.5, .group = B}},
                .nb_slots     = 3},

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
             .parts_groups = {A, B, B, A, C, B, A, A, A},
             .blason       = true,
             .amount       = 2,
             .texture      = "tile_02.png",
             .slots        = {{.x = 0.77, .y = 0.23, .group = B},
                              {.x = 0.3, .y = 0.7, .group = A}},
             .nb_slots     = 2},

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
             .parts_groups = {A, B, B, A, C, B, A, A, A},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_01.png",
             .slots        = {{.x = 0.77, .y = 0.23, .group = B},
                              {.x = 0.3, .y = 0.7, .group = A}},
             .nb_slots     = 2},

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
             .parts_groups = {A, B, E, C, F, B, D, C, A},
             .blason       = true,
             .amount       = 2,
             .texture      = "tile_24.png",
             .slots =
                 {
                     {.x = 0.77, .y = 0.23, .group = B},
                     {.x = 0.2, .y = 0.3, .group = A},
                     {.x = 0.35, .y = 0.65, .group = C},
                     {.x = 0.17, .y = 0.83, .group = D},

                 },
             .nb_slots = 4},

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
             .parts_groups = {A, B, E, C, F, B, D, C, A},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_23.png",
             .slots =
                 {
                     {.x = 0.77, .y = 0.23, .group = B},
                     {.x = 0.2, .y = 0.3, .group = A},
                     {.x = 0.35, .y = 0.65, .group = C},
                     {.x = 0.17, .y = 0.83, .group = D},

                 },
             .nb_slots = 4},

            // CFCF

            {.family = "CFCF",
             .parts  = {FIELD, TOWN, FIELD, FIELD, FIELD, FIELD, FIELD, TOWN,
                        FIELD},
             .parts_groups = {A, C, A, A, D, A, A, B, A},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_04.png",
             .slots        = {{.x = 0.5, .y = 0.1, .group = C},
                              {.x = 0.5, .y = 0.9, .group = B},
                              {.x = 0.5, .y = 0.5, .group = A}},
             .nb_slots     = 3},

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
             .parts_groups = {A, A, A, B, D, B, C, C, C},
             .blason       = false,
             .amount       = 1,
             .texture      = "tile_21.png",
             .slots =
                 {
                     {.x = 0.5, .y = 0.1, .group = A},
                     {.x = 0.5, .y = 0.5, .group = B},
                     {.x = 0.5, .y = 0.9, .group = C},

                 },
             .nb_slots = 3},

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
             .parts_groups = {A, A, A, B, B, B, C, C, C},
             .blason       = true,
             .amount       = 2,
             .texture      = "tile_22.png",
             .slots =
                 {
                     {.x = 0.5, .y = 0.1, .group = A},
                     {.x = 0.5, .y = 0.5, .group = B},
                     {.x = 0.5, .y = 0.9, .group = C},

                 },
             .nb_slots = 3},

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
             .parts_groups = {A, B, A, A, C, A, A, A, A},
             .blason       = false,
             .amount       = 5,
             .texture      = "tile_17.png",
             .slots        = {{.x = 0.5, .y = 0.1, .group = B},
                              {.x = 0.5, .y = 0.65, .group = A}},
             .nb_slots     = 2},

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
             .parts_groups = {A, B, A, C, E, A, D, C, A},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_20.png",
             .slots        = {{.x = 0.5, .y = 0.1, .group = B},
                              {.x = 0.8, .y = 0.5, .group = A},
                              {.x = 0.35, .y = 0.65, .group = C},
                              {.x = 0.17, .y = 0.83, .group = D}

             },
             .nb_slots     = 4},

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
             .parts_groups = {A, D, A, B, D, B, C, C, C},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_18.png",
             .slots        = {{.x = 0.5, .y = 0.1, .group = D},
                              {.x = 0.2, .y = 0.3, .group = A},
                              {.x = 0.5, .y = 0.5, .group = B},
                              {.x = 0.5, .y = 0.79, .group = C}},
             .nb_slots     = 4},

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
             .parts_groups = {A, D, A, A, D, B, A, B, C},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_19.png",
             .slots =
                 {
                     {.x = 0.5, .y = 0.1, .group = D},
                     {.x = 0.25, .y = 0.5, .group = A},
                     {.x = 0.65, .y = 0.65, .group = B},
                     {.x = 0.83, .y = 0.83, .group = C},
                 },
             .nb_slots = 4},

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
             .parts_groups = {G, A, G, B, H, C, F, D, E},
             .blason       = false,
             .amount       = 3,
             .texture      = "tile_03.png",
             .slots        = {{.x = 0.8, .y = 0.8, .group = E},
                              {.x = 0.2, .y = 0.8, .group = F},
                              {.x = 0.5, .y = 0.1, .group = A},
                              {.x = 0.2, .y = 0.3, .group = G},
                              {.x = 0.18, .y = 0.5, .group = B},
                              {.x = 0.82, .y = 0.5, .group = C},
                              {.x = 0.5, .y = 0.82, .group = D}},
             .nb_slots     = 7},

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
             .parts_groups = {A, A, A, A, B, A, A, A, A},
             .blason       = false,
             .amount       = 4,
             .texture      = "tile_00.png",
             .slots =
                 {
                     {.x = 0.5, .y = 0.5, .group = B},
                     {.x = 0.8, .y = 0.8, .group = A},
                 },
             .nb_slots = 2},

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
             .parts_groups = {A, A, A, A, C, A, A, B, A},
             .blason       = false,
             .amount       = 2,
             .texture      = "tile_06.png",
             .slots        = {{.x = 0.5, .y = 0.5, .group = C},
                              {.x = 0.2, .y = 0.2, .group = A},
                              {.x = 0.5, .y = 0.82, .group = B}},
             .nb_slots     = 3},

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
             .parts_groups = {A, A, A, B, D, A, C, B, A},
             .blason       = false,
             .amount       = 9,
             .texture      = "tile_08.png",
             .slots        = {{.x = 0.35, .y = 0.65, .group = B},
                              {.x = 0.7, .y = 0.3, .group = A},
                              {.x = 0.17, .y = 0.83, .group = C}},
             .nb_slots     = 3},

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
             .parts_groups = {A, A, A, B, D, B, C, C, C},
             .blason       = false,
             .amount       = 8,
             .texture      = "tile_09.png",
             .slots        = {{.x = 0.5, .y = 0.5, .group = B},
                              {.x = 0.5, .y = 0.21, .group = A},
                              {.x = 0.5, .y = 0.79, .group = C}},
             .nb_slots     = 3},

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
             .parts_groups = {A, A, A, B, G, C, E, D, F},
             .blason       = false,
             .amount       = 4,
             .texture      = "tile_10.png",
             .slots        = {{.x = 0.8, .y = 0.8, .group = F},
                              {.x = 0.2, .y = 0.8, .group = E},
                              {.x = 0.5, .y = 0.21, .group = A},
                              {.x = 0.18, .y = 0.5, .group = B},
                              {.x = 0.82, .y = 0.5, .group = C},
                              {.x = 0.5, .y = 0.82, .group = D}},
             .nb_slots     = 6},

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
             .parts_groups = {E, A, F, B, I, C, G, D, H},
             .blason       = false,
             .amount       = 1,
             .texture      = "tile_07.png",
             .slots =
                 {
                     {.x = 0.5, .y = 0.18, .group = A},
                     {.x = 0.2, .y = 0.2, .group = E},
                     {.x = 0.8, .y = 0.2, .group = F},
                     {.x = 0.18, .y = 0.5, .group = B},
                     {.x = 0.82, .y = 0.5, .group = C},
                     {.x = 0.2, .y = 0.8, .group = G},
                     {.x = 0.5, .y = 0.82, .group = D},
                     {.x = 0.8, .y = 0.8, .group = H},
                 },
             .nb_slots = 8},
};

/// @brief Vecteur des tuiles du jeu de base
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
     .parts_groups = {D, A, D, B, B, B, C, C, C},
     .blason       = false,
     .amount       = 1,
     .texture      = "tile_18.png",
     .slots =
         {
             {.x = 0.5, .y = 0.1, .group = A},
             {.x = 0.2, .y = 0.3, .group = D},
             {.x = 0.5, .y = 0.5, .group = B},
             {.x = 0.5, .y = 0.79, .group = C},
         },
     .nb_slots = 4}};

/// @brief Vecteur des tuiles de départ du jeu de base
static const tile_vector_t LIBCARCASSONNE_EXT_BASE_GAME_START_TILES = {
    .meta = {
        .size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS),
        .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS),
        .data = (tile_t*)&LIBCARCASSONNE_EXT_BASE_GAME_START_TILES_ITEMS[0]}};

/// @brief Hooks du jeu de base
static const extension_process_hook_t*
    LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST[] = {
        &hook_meeple_place, &hook_tile_place, &hook_give_back_meeples,
        &hook_next_player,  &hook_end_game,   &hook_pick_tile};

/// @brief Vecteur des hooks du jeu de base
static const extension_process_hooks_vector_t
    LIBCARCASSONNE_EXT_BASE_GAME_HOOKS = {
        .meta = {
            .size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST),
            .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST),
            .data = &LIBCARCASSONNE_EXT_BASE_GAME_HOOKS_LIST,
        }};

/// @brief Vecteur des pré-requis du jeu de base
static const extension_vector_t LIBCARCASSONNE_EXT_BASE_GAME_REQUIREMENTS = {
    .meta = {.size = 0, .caps = 0, .data = NULL}};

/// @brief Nombre de meeple de chaque joueur par type de meeple pour le jeu de
/// base
static meeple_count_t LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT[] = {
    {.count = 5, .meeple_type = BASIC}};

/// @brief Vecteur du nombre de meeple de chaque joueur par type de meeple pour
/// le jeu de base
static const meeple_count_vector_t meeples_count = {
    .meta = {.size = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT),
             .caps = SIZE(LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT),
             .data = &LIBCARCASSONNE_EXT_BASE_GAME_MEEPLE_COUNT}};

/// @brief Définition de l'extension représentant le jeu de base
const extension_t LIBCARCASSONNE_EXT_BASE_GAME = {
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

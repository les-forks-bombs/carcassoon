#pragma once

#define LIBCARCASSONNE_MAX_PLAYERS 5
#define LIBCARCASSONNE_TILES_COUNT 72

#define SUCCESS           0
#define ERROR             1
#define NOT_FREE          2
#define OUT_OF_BOUNDS     3
#define NO_TILE           4
#define INVALID_PLACEMENT 5
#define ALREADY_ALLOCATED 6

typedef char return_code_t;

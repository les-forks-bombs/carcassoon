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
#define INVALID_ACTION    7
#define NULL_POINTER      8
#define NO_MORE_PLAYER    9
#define PLAYER_NOT_CALLED 10

typedef char return_code_t;

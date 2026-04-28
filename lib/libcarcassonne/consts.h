#pragma once

#define LIBCARCASSONNE_MAX_PLAYERS 5
#define LIBCARCASSONNE_TILES_COUNT 72

#define SIZE(array) sizeof(array) / sizeof(array[0])

enum return_code {
  SUCCESS              = 0,
  ERROR                = 1,
  NOT_FREE             = 2,
  OUT_OF_BOUNDS        = 3,
  NO_TILE              = 4,
  INVALID_PLACEMENT    = 5,
  ALREADY_ALLOCATED    = 6,
  INVALID_ACTION       = 7,
  NULL_POINTER         = 8,
  NO_MORE_PLAYER       = 9,
  PLAYER_NOT_CALLED    = 10,
  NO_PROGRESS          = 11,
  REMOVE_TILE_NOT_VOID = 12,
};

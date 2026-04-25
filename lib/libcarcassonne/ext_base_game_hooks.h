#pragma once

#include <libcarcassonne/action.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/extension.h>
#include <libcarcassonne/tile.h>

typedef struct meeple_place_hook_state {
  int               x, y;
  tile_part_group_t group;
} meeple_place_hook_state_t;

return_code_t meeple_place_fw(void** state_store, engine_t* engine,
                              action_t* action);

return_code_t meeple_place_bw(void** state_store, engine_t* engine);

return_code_t meeple_place_state_free(void* state_store);

static const extension_process_hook_t meeple_place = {
    .fw       = &meeple_place_fw,
    .bw       = &meeple_place_bw,
    .free     = &meeple_place_state_free,
    .priority = 5};

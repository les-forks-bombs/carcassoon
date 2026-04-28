#pragma once

#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine_state.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/meeple.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>

#include "libcarcassonne/action.h"

struct extension_process_hook {
  const unsigned int           priority;
  const extension_forward_t    fw;
  const extension_backward_t   bw;
  const extension_free_state_t free;
  action_type_t                needed_action;
};

struct extension {
  const char                             *name;
  const tile_vector_t                    *tiles;
  const tile_vector_t                    *start_tiles;
  const unsigned int                      start_tiles_priority;
  const extension_process_hooks_vector_t *hooks;
  const extension_vector_t               *required;
  const meeple_count_vector_t             meeples_count;
};

return_code_t create_extension_list(extension_vector_t *);
void          destroy_extension_list(extension_vector_t *);

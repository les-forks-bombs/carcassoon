#pragma once

#include <libcarcassonne/forward.h>
#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine_state.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>



struct extension_process_hook {
  const unsigned int           priority;
  const extension_forward_t    fw;
  const extension_backward_t   bw;
  const extension_free_state_t free;
  engine_state_t               needed_state;
};

struct extension {
  const char                      *name;
  const tile_list_t               *tiles;
  const tile_list_t               *start_tiles;
  const unsigned int               start_tiles_priority;
  const extension_process_hooks_t *hooks;
  const extension_list_t          *required;
};

return_code_t create_extension_list(extension_list_t *);
void          destroy_extension_list(extension_list_t *);

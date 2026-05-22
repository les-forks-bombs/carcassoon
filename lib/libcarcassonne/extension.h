#pragma once

#include <libcarcassonne/consts.h>
#include <libcarcassonne/forward.h>
#include <libcarcassonne/meeple.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>

#include "libcarcassonne/action.h"

struct extension_process_hook {
  const unsigned int             priority;
  const char                     label[50];
  const extension_forward_t      fw;
  const extension_backward_t     bw;
  const extension_free_t         free;
  const extension_list_actions_t list_actions;
  action_type_t                  needed_action;
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

#define LIBCARCASSONNE_HOOK_DEF(name, pr, naction)                 \
  return_code_t name##_fw(void **state_store, engine_t *engine,    \
                          action_t *action);                       \
  return_code_t name##_bw(void **state_store, engine_t *engine);   \
  return_code_t name##_free(void **state_store, engine_t *engine); \
  return_code_t name##_list_actions(action_vector_t *actions,      \
                                    engine_t        *engine);      \
  static const extension_process_hook_t hook_##name = {            \
      .label         = #name,                                      \
      .fw            = &name##_fw,                                 \
      .bw            = &name##_bw,                                 \
      .free          = &name##_free,                               \
      .list_actions  = &name##_list_actions,                       \
      .needed_action = (naction),                                  \
      .priority      = (pr)};

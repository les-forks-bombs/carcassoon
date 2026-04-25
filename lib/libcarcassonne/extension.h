#pragma once

#include <libcarcassonne/consts.h>
#include <libcarcassonne/engine_state.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>

typedef Vector(const struct extension_process_hook *) extension_process_hooks_t;
typedef Vector(struct extension *) extension_list_t;

struct engine;
typedef struct engine engine_t;
struct action;
typedef struct action action_t;

typedef return_code_t (*extension_forward_t)(void    **state, engine_t *,
                                             action_t *action);
typedef return_code_t (*extension_backward_t)(void **state, engine_t *);
typedef return_code_t (*extension_free_state_t)(void *state);

typedef struct extension_process_hook {
  const unsigned int           priority;
  const extension_forward_t    fw;
  const extension_backward_t   bw;
  const extension_free_state_t free;
  engine_state_t               needed_state;
} extension_process_hook_t;

typedef struct extension {
  const char                      *name;
  const tile_list_t               *tiles;
  const tile_list_t               *start_tiles;
  const unsigned int               start_tiles_priority;
  const extension_process_hooks_t *hooks;
  const extension_list_t          *required;
} extension_t;

return_code_t create_extension_list(extension_list_t *);
void          destroy_extension_list(extension_list_t *);

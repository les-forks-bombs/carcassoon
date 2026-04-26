#pragma once

#include <libcarcassonne/meeple.h>
#include <libcarcassonne/tile.h>
#include <libutils/vector.h>

struct engine;

typedef int (*extension_process_t)(struct engine*);
typedef struct extension_process_hook {
  const unsigned int        priority;
  const extension_process_t fn;
} extension_process_hook_t;

typedef Vector(const extension_process_hook_t*) extension_process_hooks_t;
typedef Vector(struct extension*) extension_list_t;

typedef struct extension {
  const char*                      name;
  const tile_list_t*               tiles;
  const tile_list_t*               start_tiles;
  const unsigned int               start_tiles_priority;
  const extension_process_hooks_t* hooks;
  const extension_list_t*          required;
  const meeple_count_list_t        meeples_count;
} extension_t;

return_code_t create_extension_list(extension_list_t*);
void          destroy_extension_list(extension_list_t*);

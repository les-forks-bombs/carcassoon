#pragma once

#include <libcarcassonne/tile.h>

struct engine;

typedef int (*extension_process_t)(struct engine*);
typedef struct extension_process_hook {
  const unsigned int        priority;
  const extension_process_t fn;
} extension_process_hook_t;

typedef struct extension_process_hooks {
  const unsigned int              size;
  const extension_process_hook_t* hooks;
} extension_process_hooks_t;

typedef struct extensions_list {
  unsigned int            size;
  const struct extension* extensions;
} extension_list_t;

typedef struct extension {
  const char*                      name;
  const extension_tiles_t*         tiles;
  const extension_tiles_t*         start_tiles;
  const unsigned int               start_tiles_priority;
  const extension_process_hooks_t* hooks;
  const extension_list_t*          required;
} extension_t;

return_code_t create_extension_list(extension_list_t*);
void          destroy_extension_list(extension_list_t*);

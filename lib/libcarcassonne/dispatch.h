#pragma once

#include <libcarcassonne/forward.h>

struct dispatch {
  void*                           state_store;
  action_t*                       action;
  const extension_process_hook_t* hook;
};

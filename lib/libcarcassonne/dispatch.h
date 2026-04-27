#pragma once

#include <libcarcassonne/forward.h>

struct dispatch {
  void*     state_store;
  action_t* action;
};
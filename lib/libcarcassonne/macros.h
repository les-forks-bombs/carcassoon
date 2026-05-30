#pragma once

#define LIBCARCASSONNE_MAX_PLAYERS    5
#define LIBCARCASSONNE_TILES_COUNT    72
#define ForwardDefinition(type, name) typedef type name name##_t

#define SIZE(array) (sizeof(array) / sizeof((array)[0]))

/// @brief Nombre de sous-groupes dans une tuile
#define PLACED_TILE_GROUP_NUMBER 9

#define LIBCARCASSONNE_HOOK_HEADER(name)                           \
  return_code_t name##_fw(void **state_store, engine_t *engine,    \
                          action_t *action);                       \
  return_code_t name##_bw(void **state_store, engine_t *engine);   \
  return_code_t name##_free(void **state_store, engine_t *engine); \
  return_code_t name##_list_actions(action_vector_t *actions,      \
                                    engine_t        *engine);      \
  extern const extension_process_hook_t hook_##name;

#define LIBCARCASSONNE_HOOK_IMPL(name, pr, naction) \
  const extension_process_hook_t hook_##name = {    \
      .label         = #name,                       \
      .fw            = &name##_fw,                  \
      .bw            = &name##_bw,                  \
      .free          = &name##_free,                \
      .list_actions  = &name##_list_actions,        \
      .needed_action = (naction),                   \
      .priority      = (pr)};

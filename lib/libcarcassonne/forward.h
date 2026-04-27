#pragma once

#include <libcarcassonne/consts.h>
#include <libutils/lc.h>
#include <libutils/vector.h>

#define ForwardDefinition(type, name) typedef type name name##_t

ForwardDefinition(struct, tile_temp);
ForwardDefinition(struct, engine);
ForwardDefinition(struct, action);
ForwardDefinition(struct, action_place_tile);
ForwardDefinition(struct, action_place_meeple);
ForwardDefinition(union, action_order);
ForwardDefinition(struct, deck);
ForwardDefinition(struct, game);
ForwardDefinition(struct, meeple);
ForwardDefinition(struct, meeple_count);
ForwardDefinition(struct, options);
ForwardDefinition(struct, placed_tile_group);
ForwardDefinition(struct, placed_tile);
ForwardDefinition(struct, player);
ForwardDefinition(struct, prng_mersenne_twister_state);
ForwardDefinition(struct, tile);
ForwardDefinition(struct, vector2d);
ForwardDefinition(struct, extension);
ForwardDefinition(struct, extension_process_hook);
ForwardDefinition(struct, meeple_place_hook_state);

typedef List(const tile_t *) deck_list_t;
typedef List(placed_tile_t *) placed_tile_list_t;
typedef Vector(const extension_process_hook_t *) extension_process_hooks_t;
typedef Vector(extension_t *) extension_list_t;
typedef Vector(const tile_t) tile_list_t;
typedef Vector(placed_tile_group_t *) placed_tile_group_neighbors_t;
typedef Vector(meeple_count_t) meeple_count_list_t;
typedef Vector(meeple_t *) meeple_list_t;

typedef return_code_t (*extension_forward_t)(void    **state, engine_t *,
                                             action_t *action);
typedef return_code_t (*extension_backward_t)(void **state, engine_t *);
typedef return_code_t (*extension_free_state_t)(void *state);

#pragma once

#include <libcarcassonne/consts.h>
#include <libutils/lc.h>
#include <libutils/vector.h>
#include <libutils/hashmap.h>

#define ForwardDefinition(type, name) typedef type name name##_t

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
ForwardDefinition(struct, dispatch);
ForwardDefinition(struct, tile_place_hook_state);
ForwardDefinition(struct, rendre_meeple_hook_state);
ForwardDefinition(struct, prochain_joueur_state);
ForwardDefinition(struct, end_game_hook_state);
ForwardDefinition(struct, placed_tile_group_eval_points);
ForwardDefinition(struct, tile_slot);

typedef List(const tile_t *) deck_list_t;
typedef List(placed_tile_t *) placed_tile_list_t;
typedef List(const extension_process_hook_t *) extension_process_hooks_list_t;
typedef Vector(action_t) action_vector_t;
typedef Vector(const extension_process_hook_t *)
    extension_process_hooks_vector_t;
typedef Vector(extension_t *) extension_vector_t;
typedef Vector(const tile_t) tile_vector_t;
typedef Vector(placed_tile_group_t *) placed_tile_group_neighbors_t;
typedef Vector(meeple_count_t) meeple_count_vector_t;
typedef Vector(meeple_t *) meeple_vector_t;
typedef Vector(dispatch_t) dispatch_vector_t;
typedef Vector(placed_tile_group_eval_points_t)
    placed_tile_group_eval_points_vector_t;
typedef Vector(vector2d_t) vector2d_vector_t;

typedef HashMap(unsigned int,unsigned int) players_scores_t;

typedef enum return_code return_code_t;

typedef return_code_t (*extension_forward_t)(void    **state, engine_t *,
                                             action_t *action);
typedef return_code_t (*extension_backward_t)(void **state, engine_t *);
typedef return_code_t (*extension_free_t)(void **state, engine_t *);
typedef return_code_t (*extension_list_actions_t)(action_vector_t *actions,
                                                  engine_t *);

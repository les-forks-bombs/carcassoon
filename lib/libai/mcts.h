#pragma once

#include <libcarcassonne/libcarcassonne.h>
#include <libutils/vector.h>

struct mcts_node;
typedef struct mcts_node mcts_node_t;
typedef Vector(mcts_node_t *) mcts_node_children_t;

struct mcts_node {
  int                  visits;
  int                  score;
  action_t             action;
  mcts_node_t         *parent;
  mcts_node_children_t children;
};

void ai_play_turn(engine_t *engine, int max_iterations);

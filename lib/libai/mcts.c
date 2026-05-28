#include <cmocka.h>
#include <libai/mcts.h>
#include <libutils/prng_mersenne_twister.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROLLOUT_DEPTH 20

// -----------------------------------------------------------------------
// create_mcts_node
// -----------------------------------------------------------------------

static mcts_node_t *create_mcts_node(action_t action, mcts_node_t *parent) {
  mcts_node_t *node = calloc(1, sizeof(mcts_node_t));
  node->visits      = 0;
  node->score       = 0;
  node->action      = action;
  node->parent      = parent;
  // node->children est zero-initialisé par calloc (vecteur vide)
  if (parent != NULL) {
    vector_append(&parent->children, &node);
  }
  return node;
}

// -----------------------------------------------------------------------
// evaluate
// -----------------------------------------------------------------------

static double evaluate(engine_t *engine, player_t *current) {
  (void)engine;
  (void)current;
  return 1.0;  // Evaluation temporaire
}

// -----------------------------------------------------------------------
// ucb1
// -----------------------------------------------------------------------

static double ucb1(int total_iterations, mcts_node_t *node) {
  // INFINITY est une macro de <math.h> représentant l'infini positif en
  // virgule flottante (équivalent à 1.0/0.0). Un nœud jamais visité reçoit
  // cette valeur pour garantir qu'il sera exploré en priorité.
  if (node->visits == 0) {
    return (double)INFINITY;
  }
  return (double)node->score / (double)node->visits
         + 2.0
               * sqrt(log((double)total_iterations) / (double)node->visits);
}

// -----------------------------------------------------------------------
// backpropagate
// -----------------------------------------------------------------------

static void backpropagate(mcts_node_t *node, int score) {
  node->score = score;
  node->visits++;
  while (node->parent != NULL) {
    node = node->parent;
    node->score += score;
    node->visits++;
  }
}

// -----------------------------------------------------------------------
// rollout
// -----------------------------------------------------------------------

static int rollout(engine_t *engine, mcts_node_t *node) {
  int count = 0;
  while (!is_game_finished(&engine->game) && count < MAX_ROLLOUT_DEPTH) {
    action_vector_t actions = engine_get_actions(engine);
    unsigned int    n       = vector_size(&actions);
    if (n == 0) {
      vector_free(&actions);
      break;
    }
    unsigned int idx =
        prng_mersenne_twister_random(&engine->game.deck.state) % n;
    action_t action = *vector_nth(&actions, idx);
    vector_free(&actions);
    dispatch_action(engine, action);
    count++;           
  }
  player_t *current = game_get_current_player(&engine->game);
  int       score   = (int)evaluate(engine, current);
  backpropagate(node, score);
  return score;
}

// -----------------------------------------------------------------------
// Forward declaration (mcts et expand sont mutuellement récursifs)
// -----------------------------------------------------------------------

static void mcts(engine_t *engine, mcts_node_t *node, int total_visits);

// -----------------------------------------------------------------------
// safe_revert : n'appelle engine_revert que s'il y a quelque chose à annuler.
// Nécessaire car mcts peut retourner sans avoir dispatché (best_child == NULL
// ou partie terminée sans rollout dispatch), auquel cas vector_size == epoch
// et engine_revert entrerait dans son do-while sans élément à retirer.
// -----------------------------------------------------------------------

static void safe_revert(engine_t *engine, unsigned int epoch) {
  if (vector_size(&engine->dispatchs) > epoch) {
    engine_revert(engine, epoch);
  }
}

// -----------------------------------------------------------------------
// expand
// -----------------------------------------------------------------------

static void expand(engine_t *engine, mcts_node_t *node, int total_visits) {
  if (is_game_finished(&engine->game)) {
    return;
  }
  if (node->visits == 1) {
    action_vector_t actions = engine_get_actions(engine);
    for (unsigned int i = 0; i < vector_size(&actions); i++) {
      action_t a = *vector_nth(&actions, i);
      create_mcts_node(a, node);  // "parent" du pseudo-code = node ici
    }
    vector_free(&actions);
  }
   mcts(engine, node, total_visits);
}

// -----------------------------------------------------------------------
// mcts
// -----------------------------------------------------------------------

static void mcts(engine_t *engine, mcts_node_t *node, int total_visits) {
  if (is_game_finished(&engine->game)) {
    rollout(engine, node);
    return;
  }

  mcts_node_t *best_child = NULL;
  for (unsigned int i = 0; i < vector_size(&node->children); i++) {
    mcts_node_t *child = *vector_nth(&node->children, i);
    if (best_child == NULL
        || ucb1(total_visits, child) > ucb1(total_visits, best_child)) {
      best_child = child;
    }
  }

  if (best_child == NULL) {
    rollout(engine, node);
    return;
  }

  dispatch_action(engine, best_child->action);

  if (is_game_finished(&engine->game)) {
    player_t *current = game_get_current_player(&engine->game);
    backpropagate(best_child, (int)evaluate(engine, current));
    return;
  }

  if (best_child->visits == 0) {
    rollout(engine, best_child);
    return;
  } else {
    expand(engine, best_child, total_visits);
    return;
  }
}

// -----------------------------------------------------------------------
// mcts_free_tree
// -----------------------------------------------------------------------

static void mcts_free_tree(mcts_node_t *node) {
  for (unsigned int i = 0; i < vector_size(&node->children); i++) {
    mcts_free_tree(*vector_nth(&node->children, i));
  }
  vector_free(&node->children);
  free(node);
}

// -----------------------------------------------------------------------
// ai_play_turn
// -----------------------------------------------------------------------

void ai_play_turn(engine_t *engine, int max_iterations) {
  unsigned int epoch = vector_size(&engine->dispatchs);

  // Nœud racine (action fictive)
  action_t     dummy_action = {0};
  mcts_node_t *root         = create_mcts_node(dummy_action, NULL);


  // Création des enfants immédiats du root (placements de tuile possibles)
  action_vector_t initial_actions = engine_get_actions(engine);
  for (unsigned int i = 0; i < vector_size(&initial_actions); i++) {
    action_t a = *vector_nth(&initial_actions, i);
    create_mcts_node(a, root);  // "parent" du pseudo-code = root
  }
  vector_free(&initial_actions);
  // Boucle principale MCTS

  while (root->visits < max_iterations) {
    mcts(engine, root, root->visits);
    safe_revert(engine, epoch);
  };

  // Sélection du meilleur placement de tuile.
  // Le pseudo-code compare (score/visits) > best_child_tile.score (pas ratio).
  mcts_node_t *best_child_tile = NULL;
  for (unsigned int i = 0; i < vector_size(&root->children); i++) {
    mcts_node_t *node = *vector_nth(&root->children, i);
    if (node->visits == 0) {
      best_child_tile = node;
      continue;
    }
    if (best_child_tile == NULL
        || (node->score / node->visits) > best_child_tile->score) {
      best_child_tile = node;
    }
  }

  // Sélection du meilleur placement de meeple parmi les enfants du meilleur
  // tile.
  mcts_node_t *best_child_meeple = NULL;
  if (best_child_tile != NULL) {
    for (unsigned int i = 0; i < vector_size(&best_child_tile->children);
         i++) {
      mcts_node_t *node = *vector_nth(&best_child_tile->children, i);
      if (node->visits == 0) {
        continue;
      }
      if (best_child_meeple == NULL
          || (node->score / node->visits) > best_child_meeple->score) {
        best_child_meeple = node;
      }
    }
  }

  if (best_child_tile != NULL) {
    dispatch_action(engine, best_child_tile->action);
  }

  if (best_child_meeple != NULL) {
    dispatch_action(engine, best_child_meeple->action);
  } else if (best_child_tile != NULL) {
    // Fallback : MCTS n'a pas eu assez d'itérations pour expand best_child_tile
    // (trop de placements valides). On récupère les actions meeple directement
    // depuis l'engine et on joue NONE (dernier de la liste par convention).
    action_vector_t meeple_actions = engine_get_actions(engine);
    unsigned int    n              = vector_size(&meeple_actions);
    if (n > 0) {
      dispatch_action(engine, *vector_nth(&meeple_actions, n - 1));
    }
    vector_free(&meeple_actions);
  }

  mcts_free_tree(root);
}

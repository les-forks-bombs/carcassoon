#include <libai/mcts.h>
#include <libcarcassonne/ext_base_game_hooks.h>
#include <libutils/prng_mersenne_twister.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief Profondeur maximale d'un rollout de MCTS
#define MAX_ROLLOUT_DEPTH 10

/// @brief Alloue et initialise un nœud MCTS
/// @param action L'action associée à ce nœud
/// @param parent Le nœud parent, ou NULL si c'est la racine
/// @return Un pointeur vers le nœud créé
/// @details Le nœud est automatiquement ajouté aux enfants du parent si
/// celui-ci n'est pas NULL
static mcts_node_t *create_mcts_node(action_t action, mcts_node_t *parent) {
  mcts_node_t *node = calloc(1, sizeof(mcts_node_t));
  node->visits      = 0;
  node->score       = 0;
  node->action      = action;
  node->parent      = parent;
  if (parent != NULL) {
    vector_append(&parent->children, &node);
  }
  return node;
}

/// @brief Évalue l'état courant du jeu du point de vue de l'IA
/// @param engine Le moteur de jeu
/// @param ai_player Le joueur IA dont on évalue la position
/// @return La différence entre le score de l'IA et le meilleur score adverse -
/// cela permet de maximiser l'écart plutôt que le score brut, car il peut être
/// plus intéressant de réduire le score d'un adversaire que d'augmenter le sien
/// @details Si la partie n'est pas encore terminée, simule la fin de partie
/// pour
///          calculer les scores finaux, puis annule cette simulation
static double evaluate(engine_t *engine, player_t *ai_player) {
  void    *state_store = NULL;
  action_t dummy       = {0};
  bool did_simulate    = false;  // Indique si on a dû simuler la fin de partie
                                 // pour évaluer les scores

  if (!is_game_finished(&engine->game)) {
    engine->game.state = GAME_STATE_FINISHED;
    end_game_fw(&state_store, engine, &dummy);
    did_simulate = true;
  }

  int          my_score = (int)engine->game.players[ai_player->id].score;
  int          max_opp  = 0;
  unsigned int nb       = engine->game.options->players;
  for (unsigned int i = 0; i < nb; i++) {
    if (i != ai_player->id) {
      int s = (int)engine->game.players[i].score;
      if (s > max_opp) {
        max_opp = s;
      }
    }
  }

  if (did_simulate) {  // Si on a simulé la fin de partie, il faut annuler cette
                       // simulation pour restaurer l'état du jeu
    end_game_bw(&state_store, engine);
    end_game_free(&state_store, engine);
  }

  return (double)(my_score - max_opp);
}

/// @brief Calcule la valeur UCB1 d'un nœud pour guider la sélection
/// @param total_iterations Le nombre total de visites depuis la racine
/// @param node Le nœud dont on calcule la valeur UCB1
/// @return La valeur UCB1 du nœud (INFINITY si jamais visité, qui est un nombre
/// réel plus grand que tous les autres, garantissant que les nœuds non visités
/// seront explorés en priorité)
/// @details UCB1 = score/visites + 2 * sqrt(ln(total) / visites).
///          Un nœud non visité reçoit une priorité infinie pour garantir son
///          exploration.
static double ucb1(int total_iterations, mcts_node_t *node) {
  if (node->visits == 0) {
    return (double)INFINITY;
  }
  return (double)node->score /
         ((double)node->visits +
          (2.0 * sqrt(log((double)total_iterations) / (double)node->visits)));
}

/// @brief Propage un score vers le haut dans l'arbre depuis un nœud jusqu'à la
/// racine
/// @param node Le nœud de départ de la rétropropagation
/// @param score Le score à propager
static void backpropagate(mcts_node_t *node, int score) {
  node->score += score;
  node->visits++;
  while (node->parent != NULL) {
    node = node->parent;
    node->score += score;
    node->visits++;
  }
}

/// @brief Effectue un rollout aléatoire depuis l'état courant, c'est à dire une
/// simulation aléatoire de la partie jusqu'à une profondeur maximale ou la fin
/// de partie, puis évalue la position obtenue et propage le score dans l'arbre
/// @param engine Le moteur de jeu (modifié puis restauré par l'appelant)
/// @param node Le nœud depuis lequel débute le rollout
/// @param ai_player Le joueur IA dont on évalue la position en fin de rollout
/// @return Le score obtenu à l'issue du rollout
/// @details Joue au plus MAX_ROLLOUT_DEPTH actions aléatoires, évalue la
/// position
///          obtenue, puis appelle backpropagate pour mettre à jour l'arbre
static int rollout(engine_t *engine, mcts_node_t *node, player_t *ai_player) {
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
  int score = (int)evaluate(engine, ai_player);
  backpropagate(node, score);
  return score;
}

static void mcts(engine_t *engine, mcts_node_t *node, int total_visits,
                 player_t *ai_player);

/// @brief Développe un nœud en créant ses enfants puis poursuit la recherche
/// @param engine Le moteur de jeu
/// @param node Le nœud à développer
/// @param total_visits Le nombre total de visites depuis la racine
/// @param ai_player Le joueur IA
/// @details Les enfants ne sont créés qu'au deuxième passage sur ce nœud
/// (visits == 1), il faut d'abord au moins un rollout sur le noeud.
///          Si la partie est terminée, la fonction retourne sans rien faire.
static void expand(engine_t *engine, mcts_node_t *node, int total_visits,
                   player_t *ai_player) {
  if (is_game_finished(&engine->game)) {
    return;
  }
  if (node->visits == 1) {
    action_vector_t actions = engine_get_actions(engine);
    for (unsigned int i = 0; i < vector_size(&actions); i++) {
      action_t a = *vector_nth(&actions, i);
      create_mcts_node(a, node);
    }
    vector_free(&actions);
  }
  mcts(engine, node, total_visits, ai_player);
}

/// @brief Effectue une itération de MCTS depuis un nœud donné
/// @param engine Le moteur de jeu
/// @param node Le nœud courant dans l'arbre
/// @param total_visits Le nombre total de visites depuis la racine (pour UCB1)
/// @param ai_player Le joueur IA
/// @details Sélectionne le meilleur enfant selon UCB1, applique son action,
///          puis effectue un rollout ou développe l'enfant selon son nombre de
///          visites. Si le nœud n'a pas d'enfants (s'il n'a pas encore été
///          développé ou si c'est un noeud termnial de la partie), effectue un
///          rollout directement.
static void mcts(engine_t *engine, mcts_node_t *node, int total_visits,
                 player_t *ai_player) {
  if (is_game_finished(&engine->game)) {
    rollout(engine, node, ai_player);
    return;
  }

  mcts_node_t *best_child = NULL;
  for (unsigned int i = 0; i < vector_size(&node->children); i++) {
    mcts_node_t *child = *vector_nth(&node->children, i);
    if (best_child == NULL ||
        ucb1(total_visits, child) > ucb1(total_visits, best_child)) {
      best_child = child;
    }
  }

  if (best_child == NULL) {
    rollout(engine, node, ai_player);
    return;
  }

  dispatch_action(engine, best_child->action);

  if (is_game_finished(&engine->game)) {
    backpropagate(best_child, (int)evaluate(engine, ai_player));
    return;
  }

  if (best_child->visits == 0) {
    rollout(engine, best_child, ai_player);
  } else {
    expand(engine, best_child, total_visits, ai_player);
  }
}

/// @brief Libère récursivement toute la mémoire utilisée pour MCTS
/// @param node La racine à libérer
static void mcts_free_tree(mcts_node_t *node) {
  for (unsigned int i = 0; i < vector_size(&node->children); i++) {
    mcts_free_tree(*vector_nth(&node->children, i));
  }
  vector_free(&node->children);
  free(node);
}

/// @brief Fait jouer l'IA pour le tour courant en utilisant l'algorithme MCTS
/// @param engine Le moteur de jeu représentant l'état courant de la partie
/// @param max_iterations Le nombre maximal d'itérations de simulation à
/// effectuer
/// @details Construit un arbre MCTS en simulant max_iterations parties depuis
///          l'état courant, sélectionne le meilleur placement de tuile puis le
///          meilleur placement de meeple selon le ratio score/visites, et les
///          joue.

void ai_play_turn(engine_t *engine, int max_iterations) {
  player_t    *ai_player = game_get_current_player(&engine->game);
  unsigned int epoch     = vector_size(&engine->dispatchs);

  action_t     dummy_action = {0};
  mcts_node_t *root         = create_mcts_node(dummy_action, NULL);

  action_vector_t initial_actions = engine_get_actions(engine);
  for (unsigned int i = 0; i < vector_size(&initial_actions); i++) {
    action_t a = *vector_nth(&initial_actions, i);
    create_mcts_node(a, root);
  }
  vector_free(&initial_actions);

  while (root->visits < max_iterations) {
    mcts(engine, root, root->visits, ai_player);
    engine_revert(engine, epoch);
  }

  mcts_node_t *best_child_tile = NULL;
  double       best_tile_ratio = -INFINITY;
  for (unsigned int i = 0; i < vector_size(&root->children); i++) {
    mcts_node_t *node = *vector_nth(&root->children, i);
    if (node->visits == 0) {
      continue;
    }
    double ratio = (double)node->score / (double)node->visits;
    if (best_child_tile == NULL || ratio > best_tile_ratio) {
      best_child_tile = node;
      best_tile_ratio = ratio;
    }
  }
  if (best_child_tile == NULL && vector_size(&root->children) > 0) {
    best_child_tile = *vector_nth(&root->children, 0);
  }

  mcts_node_t *best_child_meeple = NULL;
  double       best_meeple_ratio = -INFINITY;
  if (best_child_tile != NULL) {
    for (unsigned int i = 0; i < vector_size(&best_child_tile->children); i++) {
      mcts_node_t *node = *vector_nth(&best_child_tile->children, i);
      if (node->visits == 0) {
        continue;
      }

      double ratio = (double)node->score / (double)node->visits;
      if (best_child_meeple == NULL || ratio > best_meeple_ratio) {
        best_child_meeple = node;
        best_meeple_ratio = ratio;
      }
    }
  }

  if (best_child_tile != NULL) {
    dispatch_action(engine, best_child_tile->action);
  }

  if (best_child_meeple != NULL) {
    dispatch_action(engine, best_child_meeple->action);
  } else if (best_child_tile != NULL) {
    action_vector_t meeple_actions = engine_get_actions(engine);
    unsigned int    n              = vector_size(&meeple_actions);
    if (n > 0) {
      dispatch_action(engine, *vector_nth(&meeple_actions, 0));
    }
    vector_free(&meeple_actions);
  }

  mcts_free_tree(root);
}

#include <libcarcassonne/placed_tile.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

return_code_t placed_tile_get_face(placed_face_groups_t* ret,
                                   const placed_tile_t*  tile,
                                   tile_orientation_t    connexion_face) {
  if (tile == NULL) return ERROR;

  int index = (connexion_face + tile->orientation) % 4;

  static const int values[4][3] = {{0, 1, 2}, {2, 5, 8}, {8, 7, 6}, {6, 3, 0}};

  tile_part_group_t groups[3] = {tile->parent->parts_groups[values[index][0]],
                                 tile->parent->parts_groups[values[index][1]],
                                 tile->parent->parts_groups[values[index][2]]};

  ret->face[0] = tile->groups[groups[0]];
  ret->face[1] = tile->groups[groups[1]];
  ret->face[2] = tile->groups[groups[2]];

  return SUCCESS;
}

return_code_t placed_tile_create(placed_tile_t*     placed_tile,
                                 const tile_t*      parent,
                                 tile_orientation_t orientation) {
  if (placed_tile == NULL || parent == NULL) return ERROR;

  placed_tile->parent      = parent;
  placed_tile->orientation = orientation;
  for (int i = 0; i < 9; i++) {
    placed_tile_group_t** group =
        &placed_tile->groups[placed_tile->parent->parts_groups[i]];
    if (*group == NULL) {
      *group         = calloc(1, sizeof(placed_tile_group_t));
      (*group)->tile = placed_tile;
    }
  }

  return SUCCESS;
}

void placed_tile_destroy(placed_tile_t* placed_tile) {
  bool freed[9];
  memset(&freed, 0, sizeof(freed));

  for (int i = 0; i < 9; i++) {
    int group = placed_tile->parent->parts_groups[i];
    if (!freed[group]) free(placed_tile->groups[group]);
    freed[group] = true;
  }
}

void placed_tile_group_access(placed_tile_group_t* x) {
  // le but de cette fonction est de créer le "chemin préféré"
  // au sein du splay tree

  placed_tile_group_t* w    = NULL;
  placed_tile_group_t* node = x;

  // on continue l'algorithme jusqu'a que notre noeud actuel soit nul
  while (node != NULL) {
    // on rémène notre élément a la racine du splay tree
    placed_tile_group_splay(node);

    // vu qu'on va retirer un lien fort, je retire node.right donc je l'ajoute
    // de la liste des liaisons faibles
    if (node->right != NULL)
      placed_tile_group_weak_childs_add(node, node->right);

    // vu que je crée une liaison forte vers w, je le supprime
    // de la liste des liaisons faibles
    if (w != NULL) placed_tile_group_weak_childs_rem(node, w);

    node->right = w;

    // comme les voisins ont changés, on mets a jour les valeurs
    placed_tile_group_path_aggregate(node);

    w    = node;
    node = node->parent;
  }

  placed_tile_group_splay(x);
}

placed_tile_group_t* placed_tile_group_find_root(placed_tile_group_t* node) {
  // on reconstruis le chemin préféré
  placed_tile_group_access(node);

  // remonter jusqu'a l'ancêtre le plus profond (proche si on est une feuille)
  while (node->left != NULL) node = node->left;

  placed_tile_group_splay(node);

  return node;
}

void placed_tile_group_path_aggregate(placed_tile_group_t* node) {
  node->groupe_open_slots           = node->local_open_slots;
  node->global_phantom_childs_count = node->phantom_childs_count;
  node->groupe_taille               = 1;

  if (node->left != NULL) {
    node->groupe_taille     += node->left->groupe_taille;
    node->groupe_open_slots += node->left->groupe_open_slots;
    node->global_phantom_childs_count +=
        node->left->global_phantom_childs_count;
  }
  if (node->right != NULL) {
    node->groupe_taille     += node->right->groupe_taille;
    node->groupe_open_slots += node->right->groupe_open_slots;
    node->global_phantom_childs_count +=
        node->right->global_phantom_childs_count;
  }

  for (unsigned int i = 0; i < node->weak_childs_count; i++) {
    if (node->weak_childs[i] != NULL) {
      node->groupe_taille     += node->weak_childs[i]->groupe_taille;
      node->groupe_open_slots += node->weak_childs[i]->groupe_open_slots;
      node->global_phantom_childs_count +=
          node->weak_childs[i]->global_phantom_childs_count;
    }
  }
}

static placed_tile_group_t* placed_tile_group_parent_absolu_sans_splay(
    placed_tile_group_t* node) {
  while (node->parent != NULL) node = node->parent;
  return node;
}

bool placed_tile_group_trouver_phantom(placed_tile_group_t*  node,
                                       placed_tile_group_t** ret_source,
                                       placed_tile_group_t** ret_target) {
  if (node == NULL || node->global_phantom_childs_count == 0) return false;
  placed_tile_group_t* my_root =
      placed_tile_group_parent_absolu_sans_splay(node);

  for (unsigned int i = 0; i < node->phantom_childs_count; i++) {
    placed_tile_group_t* target = node->phantom_childs[i];
    // si un fantôme est désormais dans un autre groupe, on a trouvé un phantom
    // qu'on peut réparer
    if (placed_tile_group_parent_absolu_sans_splay(target) != my_root) {
      *ret_source = node;
      *ret_target = target;
      return true;
    }
  }

  // sinon, on descend là où le radar (aggregate) indique des fantômes
  // on check la gauche
  if (node->left && node->left->global_phantom_childs_count > 0) {
    if (placed_tile_group_trouver_phantom(node->left, ret_source, ret_target))
      return true;
  }

  // on check la droite
  if (node->right && node->right->global_phantom_childs_count > 0) {
    if (placed_tile_group_trouver_phantom(node->right, ret_source, ret_target))
      return true;
  }

  // on check les enfants faibles (branches secondaires)
  for (unsigned int i = 0; i < node->weak_childs_count; i++) {
    if (node->weak_childs[i] &&
        node->weak_childs[i]->global_phantom_childs_count > 0) {
      if (placed_tile_group_trouver_phantom(node->weak_childs[i], ret_source,
                                            ret_target))
        return true;
    }
  }

  return false;
}

void placed_tile_group_cut(placed_tile_group_t* node) {
  placed_tile_group_access(node);

  if (node->left != NULL) {
    // on efface le lien vers le noeud a gauche
    node->left->parent = NULL;
    node->left         = NULL;

    placed_tile_group_path_aggregate(node);
    placed_tile_group_t* source = NULL;
    placed_tile_group_t* target = NULL;
    placed_tile_group_t* root =
        placed_tile_group_parent_absolu_sans_splay(node);

    while (placed_tile_group_trouver_phantom(root, &source, &target)) {
      placed_tile_group_phantom_childs_rem(source, target);
      placed_tile_group_phantom_childs_rem(target, source);
      placed_tile_group_link(source, target);
    }
  }
}

void placed_tile_group_link(placed_tile_group_t* a, placed_tile_group_t* b) {
  // on part du postulat que "b" est la racine de son splay tree (aka. il a été
  // splay)
  if (a == NULL || b == NULL) return;
  if (placed_tile_group_find_root(a) == placed_tile_group_find_root(b)) {
    // si on crée une boucle on l'ajoute comme arrête "fantome"
    placed_tile_group_phantom_childs_add(a, b);
    placed_tile_group_phantom_childs_add(b, a);
    return;
  }

  placed_tile_group_makeroot(a);

  a->parent = b;

  placed_tile_group_access(b);
}

bool placed_tile_group_splay_is_root(placed_tile_group_t* node) {
  // un noeud est parent ssi il n'a pas de parent
  // ou si il n'est pas relié a son parent (lien faible)

  if (node->parent == NULL) return true;

  if (node->parent->left != node      // on est pas a gauche
      && node->parent->right != node  // on est pas a droite
  ) {
    // ça veut dire qu'on a un parent, mais que ce dernier
    // n'a pas de référence vers nous, donc on a une liaison
    // "faible" aka. pas dans le meme splay tree, c'est une liaison
    // entre deux arbres séparés

    return true;
  }

  return false;
}

void placed_tile_group_rotate(placed_tile_group_t* node) {
  placed_tile_group_t* y          = node->parent;
  placed_tile_group_t* z          = y->parent;
  bool                 x_is_right = (y->right == node);

  if (z != NULL && placed_tile_group_splay_is_root(y)) {
    placed_tile_group_weak_childs_rem(z, y);
    placed_tile_group_weak_childs_add(z, node);
  }

  if (x_is_right) {
    y->right = node->left;
    if (node->left != NULL) node->left->parent = y;
  } else {
    y->left = node->right;
    if (node->right != NULL) node->right->parent = y;
  }

  node->parent = z;

  if (!placed_tile_group_splay_is_root(y)) {
    if (z->left == y)
      z->left = node;
    else
      z->right = node;
  }

  if (x_is_right)
    node->left = y;
  else
    node->right = y;
  y->parent = node;

  placed_tile_group_path_aggregate(y);
  placed_tile_group_path_aggregate(node);
}

void placed_tile_group_splay(placed_tile_group_t* node) {
  // fonction qui a pour but de faire des rotations tq. le noeud donné
  // et en racine du splay tree

  // a chaque itération, on essaie de rotate l'arbre pour finir comme on veut
  // dans le splay tree on utilise un zig-zig (deux rotations a gauche)
  // ou un zig-zag (une rotation a gauche puis a droite)

  // notre but est de le faire remonter,
  // donc tant qu'il n'est pas la racine
  while (!placed_tile_group_splay_is_root(node)) {
    placed_tile_group_t* p1 = node->parent;
    placed_tile_group_t* p2 = p1->parent;

    // si notre parent a un parent
    if (!placed_tile_group_splay_is_root(p1)) {
      bool p1_on_the_right   = (p2->right == p1);
      bool node_on_the_right = (p1->right == node);

      // si les deux sont tous à droite ou tous a gauche (zig-zig)
      if (p1_on_the_right == node_on_the_right) {
        placed_tile_group_rotate(p1);
      } else {
        // sinon on fait un zig-zag
        placed_tile_group_rotate(node);
      }
    }

    placed_tile_group_rotate(node);
  }

  placed_tile_group_path_aggregate(node);
}

void placed_tile_group_weak_childs_add(placed_tile_group_t* node,
                                       placed_tile_group_t* el) {
  node->weak_childs[node->weak_childs_count++] = el;
}

void placed_tile_group_weak_childs_rem(placed_tile_group_t* node,
                                       placed_tile_group_t* el) {
  for (unsigned int i = 0; i < node->weak_childs_count; i++) {
    if (node->weak_childs[i] == el) {
      // on remplace l'élément actuel par le dernier élément (on s'en fiche de
      // l'ordre)
      node->weak_childs_count--;
      node->weak_childs[i] = node->weak_childs[node->weak_childs_count];
    }
  }
}

void placed_tile_group_phantom_childs_add(placed_tile_group_t* node,
                                          placed_tile_group_t* el) {
  node->phantom_childs[node->phantom_childs_count++] = el;
}

void placed_tile_group_phantom_childs_rem(placed_tile_group_t* node,
                                          placed_tile_group_t* el) {
  for (unsigned int i = 0; i < node->phantom_childs_count; i++) {
    if (node->phantom_childs[i] == el) {
      // on remplace l'élément actuel par le dernier élément (on s'en fiche de
      // l'ordre)
      node->phantom_childs_count--;
      node->phantom_childs[i] =
          node->phantom_childs[node->phantom_childs_count];
    }
  }
}

bool placed_tile_group_complete(placed_tile_group_t* group) {
  return placed_tile_group_find_root(group)->groupe_open_slots == 0;
}

void placed_tile_group_makeroot(placed_tile_group_t* node) {
  placed_tile_group_access(node);
  node->flipped_delta ^= true;
  placed_tile_group_push_down(node);
}

void placed_tile_group_push_down(placed_tile_group_t* node) {
  if (node != NULL && node->flipped_delta) {
    placed_tile_group_t* tmp = node->left;
    node->left               = node->right;
    node->right              = tmp;
    if (node->left) node->left->flipped_delta ^= true;
    if (node->right) node->right->flipped_delta ^= true;
    node->flipped_delta = false;
  }
}

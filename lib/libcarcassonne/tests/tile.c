#include <libcarcassonne/game.h>
#include <libcarcassonne/placed_tile.h>
#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>
#include <malloc.h>

// instancie un groupe vide
static placed_tile_group_t* create_test_group(void) {
  return (placed_tile_group_t*)calloc(1, sizeof(placed_tile_group_t));
}

void test_tile_group_builds(void** state) {
  (void)state;
  placed_tile_group_t* node = create_test_group();

  // Un noeud seul (= un seul splay tree) doit être la racine de lui-même
  // donc il doit être la racine de son splay tree
  assert_true(placed_tile_group_splay_is_root(node));

  // ne doit pas avoir de parent (racine de l'arbre global)
  // ni d'autre éléments
  assert_null(node->parent);
  assert_null(node->left);
  assert_null(node->right);

  // il doit être sa propre racine
  assert_ptr_equal(placed_tile_group_find_root(node), node);

  free(node);
}

void test_tile_group_link(void** state) {
  (void)state;

  // on crée les deux tile groups
  placed_tile_group_t* a = create_test_group();
  placed_tile_group_t* b = create_test_group();

  // On connecte 'a' à 'b' (a devient un enfant de b)
  placed_tile_group_link(a, b);

  // La racine de 'a' doit maintenant être 'b'
  assert_ptr_equal(placed_tile_group_find_root(a), b);

  // 'b' est toujours sa propre racine
  assert_ptr_equal(placed_tile_group_find_root(b), b);

  free(a);
  free(b);
}

void test_tile_group_dynamic_connectivity(void** state) {
  (void)state;

  // comme "test_group_link" mais avec 4 tile group

  placed_tile_group_t* n1 = create_test_group();
  placed_tile_group_t* n2 = create_test_group();
  placed_tile_group_t* n3 = create_test_group();
  placed_tile_group_t* n4 = create_test_group();

  // on crée une chaîne logique: n1 -> n2 -> n3 -> n4
  placed_tile_group_link(n1, n2);
  placed_tile_group_link(n2, n3);
  placed_tile_group_link(n3, n4);

  // peu importe d'ou on part, la racine absolue de la route est n4 (dernière
  // racine linkée)
  assert_ptr_equal(placed_tile_group_find_root(n1), n4);
  assert_ptr_equal(placed_tile_group_find_root(n2), n4);
  assert_ptr_equal(placed_tile_group_find_root(n3), n4);
  assert_ptr_equal(placed_tile_group_find_root(n4), n4);

  free(n1);
  free(n2);
  free(n3);
  free(n4);
}

void test_tile_group_cut(void** state) {
  (void)state;
  placed_tile_group_t* a = create_test_group();
  placed_tile_group_t* b = create_test_group();
  placed_tile_group_t* c = create_test_group();

  // on lie a -> b -> c
  placed_tile_group_link(a, b);
  placed_tile_group_link(b, c);

  // on vérifie que la racine est bien la dernière racine liée (c)
  assert_ptr_equal(placed_tile_group_find_root(a), c);

  // on sépare le lien b de c (on coupe son parent)
  // aka. on le splay (access) pour qu'il soit en haut de l'arbre
  // avec son enfant a gauche, et l'on coupe ce lien
  placed_tile_group_cut(b);

  // on vérifie que le c est désormais son propre groupe
  assert_ptr_equal(placed_tile_group_find_root(c), c);

  // a et b sont toujours liés, mais comment b a été splay il est désormais
  // la racine du splay tree
  assert_ptr_equal(placed_tile_group_find_root(a), b);
  assert_ptr_equal(placed_tile_group_find_root(b), b);

  free(a);
  free(b);
  free(c);
}

void test_tile_group_merge_two_trees(void** state) {
  (void)state;

  // création d'un premier arbre
  placed_tile_group_t* a1 = create_test_group();
  placed_tile_group_t* a2 = create_test_group();
  placed_tile_group_link(a1, a2);

  // création d'un autre arbre identique
  placed_tile_group_t* b1 = create_test_group();
  placed_tile_group_t* b2 = create_test_group();
  placed_tile_group_link(b1, b2);

  // on vérifie qu'ils sont bien distincts
  assert_ptr_not_equal(placed_tile_group_find_root(a1),
                       placed_tile_group_find_root(b1));

  // on merge les deux groupes
  placed_tile_group_t* root_a = placed_tile_group_find_root(a1);
  placed_tile_group_t* root_b = placed_tile_group_find_root(b1);
  placed_tile_group_link(root_a, root_b);

  // Maintenant ils doivent faire partie du meme groupe
  assert_ptr_equal(placed_tile_group_find_root(a1), b2);
  assert_ptr_equal(placed_tile_group_find_root(b1), b2);

  free(a1);
  free(a2);
  free(b1);
  free(b2);
}

void tile_get_family_face_works(void** state) {
  (void)state;
  tile_t tile = {.amount       = 1,
                 .blason       = 0,
                 .family       = "AAAA",
                 .parts        = {0, 1, 2, 3, 4, 5, 0, 1, 2},
                 .parts_groups = {1, 2, 3, 4, 5, 6, 1, 2, 7}};

  // Base orientation north

  // 0 1 2
  // 3 4 5
  // 0 1 2
  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      5);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_NORTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      3);

  // 0 3 0
  // 1 4 1
  // 2 5 2
  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      3);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
      5);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_WEST,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      1);

  // 2 1 0
  // 5 4 3
  // 2 1 0
  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      3);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_SOUTH,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      5);

  // 2 5 2
  // 1 4 1
  // 0 3 0
  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                           LIBCARCASSONNE_TILE_ORIENTATION_NORTH),
      5);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                           LIBCARCASSONNE_TILE_ORIENTATION_EAST),
      1);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                           LIBCARCASSONNE_TILE_ORIENTATION_SOUTH),
      3);

  assert_int_equal(
      tile_get_family_face(&tile, LIBCARCASSONNE_TILE_ORIENTATION_EAST,
                           LIBCARCASSONNE_TILE_ORIENTATION_WEST),
      1);
}

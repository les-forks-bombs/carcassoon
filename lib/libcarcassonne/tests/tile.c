#include <cmocka.h>
#include <libcarcassonne/libcarcassonne.h>
#include <stdio.h>

void test_tile_group_builds(void** state) {
  (void)state;
  placed_tile_group_t* node;
  placed_tile_group_create(&node, NULL, NULL);
  placed_tile_group_destory(node);
}

void test_tile_group_link(void** state) {
  (void)state;

  // on crée les deux tile groups
  placed_tile_group_t* a;
  placed_tile_group_create(&a, NULL, NULL);

  placed_tile_group_t* b;
  placed_tile_group_create(&b, NULL, NULL);

  // On connecte 'a' à 'b' (a devient un enfant de b)
  placed_tile_group_link(a, b);

  // La racine de 'a' doit maintenant être 'b'
  assert_int_equal(placed_tile_group_connected(a, b), true);

  placed_tile_group_destory(a);
  placed_tile_group_destory(b);
}

void test_tile_group_dynamic_connectivity(void** state) {
  (void)state;

  // comme "test_group_link" mais avec 4 tile group

  placed_tile_group_t* n1;
  placed_tile_group_create(&n1, NULL, NULL);
  placed_tile_group_t* n2;
  placed_tile_group_create(&n2, NULL, NULL);
  placed_tile_group_t* n3;
  placed_tile_group_create(&n3, NULL, NULL);
  placed_tile_group_t* n4;
  placed_tile_group_create(&n4, NULL, NULL);

  // on crée une chaîne logique: n1 -> n2 -> n3 -> n4
  placed_tile_group_link(n1, n2);
  placed_tile_group_link(n2, n3);
  placed_tile_group_link(n3, n4);

  // peu importe d'ou on part, la racine absolue de la route est n4 (dernière
  // racine linkée)
  assert_int_equal(placed_tile_group_connected(n1, n4), true);
  assert_int_equal(placed_tile_group_connected(n1, n4), true);
  assert_int_equal(placed_tile_group_connected(n3, n4), true);
  assert_int_equal(placed_tile_group_connected(n4, n4), true);

  placed_tile_group_destory(n1);
  placed_tile_group_destory(n2);
  placed_tile_group_destory(n3);
  placed_tile_group_destory(n4);
}

void test_tile_group_cut(void** state) {
  (void)state;
  placed_tile_group_t* a;
  placed_tile_group_create(&a, NULL, NULL);
  placed_tile_group_t* b;
  placed_tile_group_create(&b, NULL, NULL);
  placed_tile_group_t* c;
  placed_tile_group_create(&c, NULL, NULL);

  // on lie a -> b -> c
  placed_tile_group_link(a, b);
  placed_tile_group_link(b, c);

  // on vérifie que la racine est bien la dernière racine liée (c)
  assert_int_equal(placed_tile_group_connected(a, b), true);

  // on sépare le lien b de c (on coupe son parent)
  // aka. on le splay (access) pour qu'il soit en haut de l'arbre
  // avec son enfant a gauche, et l'on coupe ce lien
  placed_tile_group_cut(a, b);

  // on vérifie que le c est désormais son propre groupe
  assert_int_equal(placed_tile_group_connected(a, b), false);

  placed_tile_group_destory(a);
  placed_tile_group_destory(b);
  placed_tile_group_destory(c);
}

void test_tile_group_merge_two_trees(void** state) {
  (void)state;

  // création d'un premier arbre
  placed_tile_group_t* a1;
  placed_tile_group_create(&a1, NULL, NULL);
  placed_tile_group_t* a2;
  placed_tile_group_create(&a2, NULL, NULL);
  placed_tile_group_link(a1, a2);

  // création d'un autre arbre identique
  placed_tile_group_t* b1;
  placed_tile_group_create(&b1, NULL, NULL);
  placed_tile_group_t* b2;
  placed_tile_group_create(&b2, NULL, NULL);
  placed_tile_group_link(b1, b2);

  // on vérifie qu'ils sont bien distincts
  assert_int_equal(placed_tile_group_connected(a1, b1), false);

  // on merge les deux groupes

  placed_tile_group_link(a2, b2);

  // Maintenant ils doivent faire partie du meme groupe
  assert_int_equal(placed_tile_group_connected(a1, b1), true);

  placed_tile_group_destory(a1);
  placed_tile_group_destory(a2);
  placed_tile_group_destory(b1);
  placed_tile_group_destory(b2);
}

void tile_get_family_face_works(void** state) {
  (void)state;
  tile_t tile = {.amount       = 1,
                 .blason       = false,
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

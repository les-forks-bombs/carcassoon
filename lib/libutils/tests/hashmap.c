
#include <libutils/hashmap.h>
#include <libutils/tests/tests.h>
#include <string.h>

void utils_hashmap_create_test(void** state) {
  (void)state;
  char_char_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  hashmap_free(&map);
}

void utils_hashmap_get_set_test(void** state) {
  (void)state;
  char_int_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  char key1[] = "hehe";
  int  val1   = 42;
  assert_null(hashmap_get(&map, key1, sizeof(key1)));

  hashmap_set(&map, key1, sizeof(key1), &val1, sizeof(val1));

  int* res = hashmap_get(&map, key1, sizeof(key1));
  assert_non_null(res);
  assert_int_equal(*res, val1);

  hashmap_free(&map);
}

void utils_hashmap_string_test(void** state) {
  (void)state;
  char_char_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  // Test avec des chaînes de caractères
  char key1[] = "test_string";
  char val1[] = "value1";

  hashmap_set(&map, key1, sizeof(key1), val1, sizeof(val1));

  char* res = hashmap_get(&map, key1, sizeof(key1));
  assert_non_null(res);
  assert_string_equal(res, "value1");

  // Test avec une autre chaîne
  char key2[] = "another_key";
  char val2[] = "value2";

  hashmap_set(&map, key2, sizeof(key2), val2, sizeof(val2));

  res = hashmap_get(&map, key2, sizeof(key2));
  assert_non_null(res);
  assert_string_equal(res, "value2");

  hashmap_free(&map);
}

void utils_hashmap_pointer_test(void** state) {
  (void)state;
  char_int_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  // Test avec des pointeurs comme clés
  int  a    = 10;
  int  b    = 20;
  int* key1 = &a;
  int* key2 = &b;
  int  val1 = 100;
  int  val2 = 200;

  hashmap_set(&map, &key1, sizeof(key1), &val1, sizeof(val1));
  hashmap_set(&map, &key2, sizeof(key2), &val2, sizeof(val2));

  int* res1 = hashmap_get(&map, &key1, sizeof(key1));
  int* res2 = hashmap_get(&map, &key2, sizeof(key2));

  assert_non_null(res1);
  assert_non_null(res2);
  assert_int_equal(*res1, 100);
  assert_int_equal(*res2, 200);

  hashmap_free(&map);
}

void utils_hashmap_update_test(void** state) {
  (void)state;
  char_int_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  // Test de mise à jour d'une valeur existante
  char key[] = "update_key";
  int  val1  = 42;
  int  val2  = 100;

  hashmap_set(&map, key, sizeof(key), &val1, sizeof(val1));

  int* res = hashmap_get(&map, key, sizeof(key));
  assert_non_null(res);
  assert_int_equal(*res, 42);

  // Mise à jour avec une nouvelle valeur
  hashmap_set(&map, key, sizeof(key), &val2, sizeof(val2));

  res = hashmap_get(&map, key, sizeof(key));
  assert_non_null(res);
  assert_int_equal(*res, 100);

  hashmap_free(&map);
}

void utils_hashmap_remove_test(void** state) {
  (void)state;
  char_char_map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  // Test de suppression
  char key1[] = "key1";
  char key2[] = "key2";
  int  val1   = 10;
  int  val2   = 20;

  hashmap_set(&map, key1, sizeof(key1), &val1, sizeof(val1));
  hashmap_set(&map, key2, sizeof(key2), &val2, sizeof(val2));

  // Vérification que les clés existent
  assert_non_null(hashmap_get(&map, key1, sizeof(key1)));
  assert_non_null(hashmap_get(&map, key2, sizeof(key2)));

  // Suppression de key1
  hashmap_remove(&map, key1, sizeof(key1));

  // key1 ne devrait plus exister
  assert_null(hashmap_get(&map, key1, sizeof(key1)));
  // key2 devrait toujours exister
  assert_non_null(hashmap_get(&map, key2, sizeof(key2)));

  hashmap_free(&map);
}

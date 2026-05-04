#include <cmocka.h>
#include <libutils/hashmap.h>
#include <libutils/tests/tests.h>
#include <string.h>

void utils_hashmap_create_test(void** state) {
  map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  hashmap_free(&map);
}

void utils_hashmap_get_set_test(void** state) {
  map_t map;
  memset(&map, 0, sizeof(map));
  hashmap_create(&map, 5);

  char key1[20] = "hehe";
  int  val1     = 42;
  assert_null(hashmap_get(&map, key1));

  hashmap_set(&map, key1, &val1);

  int* res = hashmap_get(&map, key1);
  assert_int_equal(*res, val1);

  hashmap_free(&map);
}
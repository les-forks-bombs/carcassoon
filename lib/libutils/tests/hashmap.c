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
  assert_null(hashmap_get(&map, &key1));

  hashmap_free(&map);
}
#include <libutils/tests/tests.h>
#include <libutils/hashmap.h>
#include <string.h>

void utils_hashmap_create_test(void** state){
    map_t map;
    memset(&map, 0, sizeof(map));
    hashmap_create(&map, 5);

    hashmap_free(&map);
}
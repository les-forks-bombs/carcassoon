#include <libutils/tests/tests.h>
#include <libutils/hashmap.h>

void utils_hashmap_create_test(void** state){
    map_t map;
    hashmap_create(&map, 5);

    hashmap_free(&map);
}
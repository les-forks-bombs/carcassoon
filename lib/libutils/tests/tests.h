#pragma once
#include <libutils/cmocka.h>
#include <libutils/hashmap.h>
#include <libutils/lc.h>

typedef List(int) int_list_t;
typedef union {
  hashmap_t meta;
  char*     key_ghost[20];
  int*      value_ghost;
} map_t;

void utils_list_append_prepend(void** state);
void utils_vector_create(void** state);
void utils_hashmap_create_test(void** state);
void utils_hashmap_get_set_test(void** state);

static const struct CMUnitTest tests[] = {
    cmocka_unit_test(utils_list_append_prepend),
    cmocka_unit_test(utils_vector_create),
    cmocka_unit_test(utils_hashmap_create_test),
    cmocka_unit_test(utils_hashmap_get_set_test),
};
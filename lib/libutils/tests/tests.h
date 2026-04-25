#pragma once
#include <libutils/cmocka.h>
#include <libutils/lc.h>

typedef List(int) int_list_t;
void utils_list_append_prepend(void** state);
void utils_vector_create(void** state);

static const struct CMUnitTest tests[] = {
    cmocka_unit_test(utils_list_append_prepend),
    cmocka_unit_test(utils_vector_create)};
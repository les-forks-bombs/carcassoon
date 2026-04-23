#pragma once
#include <libutils/cmocka.h>
#include <libutils/lc.h>

typedef List(int) int_list_t;
void utils_list_append_prepend(void** state);

static const struct CMUnitTest tests[] = {cmocka_unit_test(utils_list_append_prepend)};
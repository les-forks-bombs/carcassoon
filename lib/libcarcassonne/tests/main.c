#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <cmocka.h>
#include <libcarcassonne/tests/tests.h>

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(deck_builds),
        cmocka_unit_test(deck_seed_consistent),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

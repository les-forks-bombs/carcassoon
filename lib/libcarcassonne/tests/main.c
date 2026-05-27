#include <cmocka.h>
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

int main(void) { return cmocka_run_group_tests(tests, NULL, NULL); }

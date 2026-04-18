#include <libcarcassonne/tests/tests.h>
#include <libutils/cmocka.h>

int main(void) {
  cmocka_set_message_output(CM_OUTPUT_STDOUT | CM_OUTPUT_XML);
  return cmocka_run_group_tests(tests, NULL, NULL);
}

#include <libai/libai.h>
#include <utils/cmocka.h>

/* Test that the program does not fail when calling `libai_doing_stuff_works` */
static void libai_doing_stuff_works(void** state) {
  ai_do_stuff();
  (void)state; /* unused */
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(libai_doing_stuff_works),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

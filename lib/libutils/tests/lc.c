#include <libutils/lc.h>
#include <libutils/tests/tests.h>

void utils_list_append_prepend(void** state) {
  (void)state;

  int_list_t list = {.meta = {.head = NULL, .tail = NULL, .size = 0}};

  assert_int_equal(list_size(&list), 0);

  static int ints[] = {0, 1, 2, 3, 4};

  list_append(&list, &ints[0]);
  list_append(&list, &ints[1]);
  list_append(&list, &ints[2]);
  list_append(&list, &ints[3]);
  list_append(&list, &ints[4]);

  for (int i = 0; i < 4; i++)
    assert_int_equal(*list_value(&list, list_nth(&list, i)), i);

  list_prepend(&list, &ints[2]);
  assert_int_equal(*list_value(&list, list_head(&list)), 2);
  assert_int_equal(*list_value(&list, list_tail(&list)), 4);

  list_insert(&list, &ints[3], 2);
  assert_int_equal(*list_value(&list, list_nth(&list, 2)), 3);
  
  assert_int_equal(list_size(&list), 7);

  list_remove_value(&list, &ints[3]);

  assert_int_equal(list_size(&list), 6);

  list_remove_value(&list, &ints[3]);
  assert_int_equal(list_size(&list), 5);


  list_free(&list);
}

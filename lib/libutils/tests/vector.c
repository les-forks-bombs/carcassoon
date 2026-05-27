#include <libutils/tests/tests.h>
#include <libutils/vector.h>

void utils_vector_create(void** state) {
  (void)state;
  Vector(int) ints = {0};
  ints.meta.size   = 0;

  vector_alloc(&ints, 1);

  int a[] = {1, 2, 3};

  // [2]
  vector_append(&ints, &a[1]);

  assert_int_equal(vector_caps(&ints), 1);

  // [2, 2]
  vector_append(&ints, &a[1]);
  // le vecteur grow de 1
  assert_int_equal(vector_caps(&ints), 2);

  // [2, 2, 2]
  vector_append(&ints, &a[1]);
  // le vecteur a grow de 2
  assert_int_equal(vector_caps(&ints), 4);
  assert_int_equal(vector_size(&ints), 3);

  // [2, 2]
  vector_remove(&ints, 0);
  assert_int_equal(vector_size(&ints), 2);

  // [2]
  vector_remove(&ints, 0);
  assert_int_equal(vector_size(&ints), 1);

  assert_int_equal(*vector_nth(&ints, 0), 2);

  vector_remove_value(&ints, &a[2]);
  assert_int_equal(vector_size(&ints), 1);

  vector_remove_value(&ints, &a[1]);
  assert_int_equal(vector_size(&ints), 0);

  vector_free(&ints);
}

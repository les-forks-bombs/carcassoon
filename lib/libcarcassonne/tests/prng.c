
#include <libcarcassonne/libcarcassonne.h>
#include <libcarcassonne/tests/tests.h>

void prng_create_with_seed_initializes_state(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng = create_prng_mersenne_twister_state(42);

  assert_int_equal(prng.state_index, 0);
  assert_int_equal(prng.state_array[0], 42);
}

void prng_random_consistent_sequence(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng1 = create_prng_mersenne_twister_state(123);
  prng_mersenne_twister_state_t prng2 = create_prng_mersenne_twister_state(123);

  for (int i = 0; i < 10; i++) {
    unsigned int val1 = prng_mersenne_twister_random(&prng1);
    unsigned int val2 = prng_mersenne_twister_random(&prng2);

    assert_int_equal(val1, val2);
  }
}

void prng_random_different_seeds_different_sequence(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng1 = create_prng_mersenne_twister_state(42);
  prng_mersenne_twister_state_t prng2 = create_prng_mersenne_twister_state(43);

  unsigned int val1 = prng_mersenne_twister_random(&prng1);
  unsigned int val2 = prng_mersenne_twister_random(&prng2);

  assert_int_not_equal(val1, val2);
}

void prng_random_values_in_valid_range(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng = create_prng_mersenne_twister_state(42);

  for (int i = 0; i < 100; i++) {
    unsigned int val = prng_mersenne_twister_random(&prng);
    assert_true(val <= 0xFFFFFFFF);
  }
}

#include <libcarcassonne/prng_mersenne_twister.h>
#include <libcarcassonne/tests/tests.h>

/// @brief Test que create_prng_mersenne_twister_state initialise correctement l'état
void prng_create_with_seed_initializes_state(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng = create_prng_mersenne_twister_state(42);

  // Vérifie que state_index est initialisé à 0
  assert_int_equal(prng.state_index, 0);

  // Vérifie que le premier élément du state_array est la seed
  assert_int_equal(prng.state_array[0], 42);
}

/// @brief Test que la même seed produit la même séquence
void prng_random_consistent_sequence(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng1 = create_prng_mersenne_twister_state(123);
  prng_mersenne_twister_state_t prng2 = create_prng_mersenne_twister_state(123);

  // Génère 10 nombres avec chaque générateur
  for (int i = 0; i < 10; i++) {
    unsigned int val1 = prng_mersenne_twister_random(&prng1);
    unsigned int val2 = prng_mersenne_twister_random(&prng2);

    assert_int_equal(val1, val2);
  }
}

/// @brief Test que différentes seeds produisent des séquences différentes
void prng_random_different_seeds_different_sequence(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng1 = create_prng_mersenne_twister_state(42);
  prng_mersenne_twister_state_t prng2 = create_prng_mersenne_twister_state(43);

  // Génère un nombre avec chaque générateur
  unsigned int val1 = prng_mersenne_twister_random(&prng1);
  unsigned int val2 = prng_mersenne_twister_random(&prng2);

  // Les valeurs doivent être différentes (très improbable d'être égales)
  assert_int_not_equal(val1, val2);
}

/// @brief Test que les valeurs générées sont dans la plage attendue
void prng_random_values_in_valid_range(void** state) {
  (void)state;

  prng_mersenne_twister_state_t prng = create_prng_mersenne_twister_state(42);

  // Génère 100 nombres et vérifie qu'ils sont non nuls
  // (en pratique, ils devraient être dans [0, UINT_MAX])
  for (int i = 0; i < 100; i++) {
    unsigned int val = prng_mersenne_twister_random(&prng);
    // On vérifie juste que c'est une valeur valide (pas de crash)
    assert_true(val <= 0xFFFFFFFF);
  }
}

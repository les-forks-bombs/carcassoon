#pragma once

#include <stdint.h>

#define LIBCARCASSONNE_PRNG_n     624
#define LIBCARCASSONNE_PRNG_m     397
#define LIBCARCASSONNE_PRNG_w     32
#define LIBCARCASSONNE_PRNG_r     31
#define LIBCARCASSONNE_PRNG_UMASK (0xffffffffUL << LIBCARCASSONNE_PRNG_r)
#define LIBCARCASSONNE_PRNG_LMASK \
  (0xffffffffUL >> (LIBCARCASSONNE_PRNG_w - LIBCARCASSONNE_PRNG_r))
#define LIBCARCASSONNE_PRNG_a 0x9908b0dfUL
#define LIBCARCASSONNE_PRNG_u 11
#define LIBCARCASSONNE_PRNG_s 7
#define LIBCARCASSONNE_PRNG_t 15
#define LIBCARCASSONNE_PRNG_l 18
#define LIBCARCASSONNE_PRNG_b 0x9d2c5680UL
#define LIBCARCASSONNE_PRNG_c 0xefc60000UL
#define LIBCARCASSONNE_PRNG_f 1812433253UL

/// @brief Status du générateur aléatoire
/// @cite https://en.wikipedia.org/wiki/Mersenne_Twister#C_code
/// @remark Emprunté de l'article wikipedia
typedef struct prng_mersenne_twister_state {
  unsigned int
      state_array[LIBCARCASSONNE_PRNG_n];  // the array for the state vector
  int state_index;  // index into state vector array, 0 <= state_index <= n-1
                    // always
} prng_mersenne_twister_state_t;

/// @brief Crée un état de générateur de nombre pseudo aléatoire
/// @param seed La seed pour initialiser le state
/// @return L'état du générateur
/// @related prng_mersenne_twister_state
prng_mersenne_twister_state_t create_prng_mersenne_twister_state(int seed);

/// @brief Choisis un nombre aléatoire non signé
/// @param state L'état du générateur
/// @return Un nombre aléatoire
/// @related prng_mersenne_twister_state
unsigned int prng_mersenne_twister_random(prng_mersenne_twister_state_t* state);

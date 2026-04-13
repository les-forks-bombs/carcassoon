#include <libcarcassonne/prng_mersenne_twister.h>
#include <memory.h>
#include <stdlib.h>

prng_mersenne_twister_state_t create_prng_mersenne_twister_state(int seed) {
  prng_mersenne_twister_state_t state;
  memset(&state, 0, sizeof(prng_mersenne_twister_state_t));

  unsigned int* state_array = &(state.state_array[0]);

  state_array[0] = seed;  // suggested initial seed = 19650218UL

  for (int i = 1; i < LIBCARCASSONNE_PRNG_n; i++) {
    seed =
        LIBCARCASSONNE_PRNG_f * (seed ^ (seed >> (LIBCARCASSONNE_PRNG_w - 2))) +
        i;  // Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
    state_array[i] = seed;
  }

  state.state_index = 0;

  return state;
}

unsigned int prng_mersenne_twister_random(
    prng_mersenne_twister_state_t* state) {
  unsigned int* state_array = &(state->state_array[0]);

  int k = state->state_index;  // point to current state location
                               // 0 <= state_index <= n-1   always

  //  int k = k - n;                   // point to state n iterations before
  //  if (k < 0) k += n;               // modulo n circular indexing
  // the previous 2 lines actually do nothing
  //  for illustration only

  int j =
      k - (LIBCARCASSONNE_PRNG_n - 1);  // point to state n-1 iterations before
  if (j < 0) j += LIBCARCASSONNE_PRNG_n;  // modulo n circular indexing

  unsigned int x = (state_array[k] & LIBCARCASSONNE_PRNG_UMASK) |
                   (state_array[j] & LIBCARCASSONNE_PRNG_LMASK);

  unsigned int xA = x >> 1;
  if (x & 0x00000001UL) xA ^= LIBCARCASSONNE_PRNG_a;

  j = k - (LIBCARCASSONNE_PRNG_n -
           LIBCARCASSONNE_PRNG_m);  // point to state n-m iterations before
  if (j < 0) j += LIBCARCASSONNE_PRNG_n;  // modulo n circular indexing

  x                = state_array[j] ^ xA;  // compute next value in the state
  state_array[k++] = x;                    // update new state value

  if (k >= LIBCARCASSONNE_PRNG_n) k = 0;  // modulo n circular indexing
  state->state_index = k;

  unsigned int y = x ^ (x >> LIBCARCASSONNE_PRNG_u);  // tempering
  y              = y ^ ((y << LIBCARCASSONNE_PRNG_s) & LIBCARCASSONNE_PRNG_b);
  y              = y ^ ((y << LIBCARCASSONNE_PRNG_t) & LIBCARCASSONNE_PRNG_c);
  unsigned int z = y ^ (y >> LIBCARCASSONNE_PRNG_l);

  return z;
}

#ifndef UNIFORM_H
#define UNIFORM_H
#include <stdio.h>

#include <stdint.h>

extern uint64_t rand15_calls;
extern uint32_t rng_seed;

#define RNG_MAX INT16_MAX  // 2^16 - 1

// Generate a 16-bit integer, where the lower 15 bits are uniformly random
static inline uint16_t rand15(void) {
    rand15_calls++;
    rng_seed = rng_seed * 747796405u + 2891336453u;
    return (rng_seed >> 17);  // return 15-bit result
}

// Generate a uniform random float on the SEMI-OPEN interval [0.0, 1.0)
static inline float frand(void) {
    return (float) rand15() / (float) ((uint32_t) RNG_MAX+1);
}

// Generate a uniform random float on the OPEN interval (0.0, 1.0)
static inline float frand_open(void) {
    return (float) ((uint32_t) rand15() + 1) / (float) ((uint32_t) RNG_MAX + 2);
}


#endif
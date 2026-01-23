#ifndef UNIFORM_H
#define UNIFORM_H

#define URAND_MAX 0x7fff
static uint16_t rng_state = 1;

// Set RNG seed
static inline void useed(uint16_t seed) {
    rng_state = seed;
    return;
}

// Sample a random 16-bit integer, URAND_MAX or lower
static inline uint16_t urand(void) {
    rng_state = rng_state * 1103515245 + 12345;
    return rng_state >> 1;  // return 15-bit result
}

// Sample uniformly on [0,1)
static inline float rand_unif_half_open(void) {
    return ((float) urand()) / ((float) (URAND_MAX + 1.0f));
}

// Sample uniformly on [0,1]
static inline float rand_unif_closed(void) {
    return ((float) urand()) / ((float) (URAND_MAX));
}

// Sample uniformly on (0,1)
static inline float rand_unif_open(void) {
    return ((float) (urand() + 1)) / ((float) (URAND_MAX + 2));
}

static inline int uniform_multiple(float* arr, int N) {
    int i;
    for (i = 0; i < N; i++) {
        arr[i] = rand_unif_half_open();
    }
    return 0;
}

#endif
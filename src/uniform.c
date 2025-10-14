#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "uniform.h"

// Sample uniformly on [0,1)
float rand_unif_half_open(void) {
    return ((float) rand()) / ((float) (RAND_MAX + 1.0f));
}

// Sample uniformly on [0,1]
float rand_unif_closed(void) {
    return ((float) rand()) / ((float) (RAND_MAX));
}

// Sample uniformly on (0,1)
float rand_unif_open(void) {
    return ((float) (rand() + 1)) / ((float) (RAND_MAX + 2));
}

int uniform_multiple(float* arr, int N) {
    int i;
    for (i = 0; i < N; i++) {
        arr[i] = rand_unif_half_open();
    }
    return 0;
}
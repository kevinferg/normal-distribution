#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "uniform.h"

// Sample uniformly on [0,1)
double rand_unif_half_open(void) {
    return ((double) rand()) / (RAND_MAX + 1.0);
}

// Sample uniformly on [0,1]
double rand_unif_closed(void) {
    return ((double) rand()) / (RAND_MAX);
}

// Sample uniformly on (0,1)
double rand_unif_open(void) {
    return ((double) (rand() + 1)) / (RAND_MAX + 2.0);
}

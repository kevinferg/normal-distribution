#ifndef UNIFORM_H
#define UNIFORM_H

// Sample uniformly on [0,1)
float rand_unif_half_open(void);

// Sample uniformly on [0,1]
float rand_unif_closed(void);

// Sample uniformly on (0,1)
float rand_unif_open(void);

int uniform_multiple(float* arr, int N);

#endif
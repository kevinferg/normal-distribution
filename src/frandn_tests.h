#ifndef TESTING_H
#define TESTING_H

#include <stdint.h>

#include "utils/uniform.h"
#define NUM_VALS (RNG_MAX * 1000) // Number of values to compute in the test

typedef float (*FloatFn)(void); // Function with no arguments, returns float

#define NAME_LEN 32 // max chars in algorithm name string: frandn_[NAME]
// Results from the algorithm test
typedef struct AlgResults {
    char   nickname[NAME_LEN];
    double nanoseconds;
    double rand_calls;         // Avg number of calls to rng function
    float  mean;     // Mean of generated samples     (ideal = 0)
    float  stdev;    // Stdev of generated samples    (ideal = 1)
    float  skewness; // Skewness of generated samples (ideal = 0)
    float  kurtosis; // Kurtosis of generated samples (ideal = 3)
} AlgResults;


// Arguments to the algorithm testing function
typedef struct AlgArgs {
    FloatFn fn; // Pointer to algorithm function
    float* arr; // Array of floats to populate
    int N;      // Number of elements in arr
} AlgArgs;


typedef struct AlgFunctionInfo {
    FloatFn fn;
    char nickname[NAME_LEN];
} AlgFunctionInfo;


AlgResults get_alg_results(char* name, FloatFn fn, int N);
void sort_results(AlgResults *arr, int N);
void print_results_summary(AlgResults *arr, int N);

#endif
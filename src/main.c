#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "timing.h"
#include "normal.h"
#include "uniform.h"
#include "stats.h"
#include "probit.h"
#include "fasttrig.h"

#define NUM_VALS (RAND_MAX*100)

void test_alg(void* args) {
    AlgArgs* A = (AlgArgs*) args;
    int i;
    int status = alg_functions[A->id](A->arr, A->N);
    return;
}

void print_summary_header(void) {
    printf("   ALGORITHM     TIME:NS          MEAN      STDDEV    SKEWNESS    KURTOSIS\n");
}

int get_method_summary(AlgID id, int N) {
    double t;
    double mean, stdev, skew, kurt;
    double* vals = malloc(N * sizeof(double));
    if (vals == NULL) return -1;

    AlgArgs A = {.id=id, .arr=vals, .N=N};
    t = time_function(&test_alg, &A);
    
    mean = get_mean(A.arr, N);
    stdev = get_stdev(A.arr, N);
    skew = get_skewness(A.arr, N);
    kurt = get_kurtosis(A.arr, N);

    // printf("------------------------------------------------\n");
    // printf("> \"%s\" Method\n",alg_names[id]);
    // printf("> Sampling %d numbers from N(0,1)\n", N);
    // printf("    Mean:  % 0.6f    "
    //        "   Stdev:  % 0.6f\n", mean, stdev);
    // printf("Skewness:  % 0.6f    "
    //        "Kurtosis:  % 0.6f\n", skew, kurt);

    // printf("Time (ms): %3.f\n", t*1000);
    // printf("------------------------------------------------\n\n");

    printf("%12s       %5.f    % 10.6f  % 10.6f  % 10.6f  % 10.6f\n", alg_names_short[id], t/(NUM_VALS)*1e9, mean, stdev, skew, kurt);

    free(vals);
    return 0;
}

int main(int argc, char** argv) {

    AlgID i;

    print_summary_header();
    for (i = 0; i < ALGMAX; i++) {
        get_method_summary(i, NUM_VALS);
    }

    // test_probit();
    // test_fasttrig();

    return 0;
}

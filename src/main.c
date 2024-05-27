#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "timing.h"
#include "normal.h"
#include "uniform.h"
#include "stats.h"
#include "probit.h"

#define NUM_VALS 10000000

void test_alg(void* args) {
    AlgArgs* A = (AlgArgs*) args;
    int i;
    int status = alg_functions[A->id](A->arr, A->N);
    return;
}

int get_method_summary(AlgID id, int N) {
    double t;
    double mean, stdev;
    double* vals = malloc(N * sizeof(double));
    if (vals == NULL) return -1;

    AlgArgs A = {.id=id, .arr=vals, .N=N};
    t = time_function(&test_alg, &A);
    
    mean = get_mean(A.arr, N);
    stdev = get_stdev(A.arr, N);

    printf("------------------------------------------------\n");
    printf("> \"%s\" Method\n",alg_names[id]);
    printf("> Sampling %d numbers from N(0,1)\n", N);
    printf("Mean:  % 0.6f\n", mean);
    printf("Stdev: % 0.6f\n", stdev);
    printf("Time (ms): %3.f\n", t*1000);
    printf("------------------------------------------------\n\n");

    free(vals);
    return 0;
}

int main(int argc, char** argv) {

    AlgID i;

    for (i = 0; i < ALGMAX; i++) {
        get_method_summary(i, NUM_VALS);
    }

    // test_probit();

    return 0;
}

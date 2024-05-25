#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "timing.h"
#include "normal.h"
#include "uniform.h"

#define NUM_VALS 1000000

void test_alg(void* args) {
    AlgArgs* A = (AlgArgs*) args;
    int i;
    int status = alg_functions[A->id](A->arr, A->N);
    return;
}

int get_method_summary(AlgID id, int N) {
    double t;
    double* vals = malloc(N * sizeof(double));
    if (vals == NULL) return -1;

    AlgArgs A = {.id=id, .arr=vals, .N=N};
    t = time_function(&test_alg, &A);

    printf("-------- Method \"%s\" --------\n",alg_names[id]);
    printf("Sampling %d numbers\n", N);
    printf("Time (ms): %f\n\n", t);

    free(vals);
    return 0;
}

int main(int argc, char** argv) {
    AlgID i;

    for (i = 0; i < ALGMAX; i++) {
        get_method_summary(i, NUM_VALS);
    }

    return 0;
}

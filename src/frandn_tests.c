#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils/timing.h"
#include "utils/stats.h"
#include "utils/uniform.h"
#include "frandn_tests.h"

static void test_alg(void* args) {
    AlgArgs* A = (AlgArgs*) args;
    FloatFn f = A->fn;
    int i;
    for (i = 0; i < A->N; i++) {
        A->arr[i] = (*f)();
    }
    return;
}

static int compare_results_by_time(const void *a, const void *b) {
    const AlgResults *ra = (const AlgResults *)a;
    const AlgResults *rb = (const AlgResults *)b;

    if (ra->nanoseconds < rb->nanoseconds) return 1;
    if (ra->nanoseconds > rb->nanoseconds) return -1;
    return 0;
}

void sort_results(AlgResults *arr, int N) {
    qsort(arr, N, sizeof(AlgResults), compare_results_by_time);
}


AlgResults get_alg_results(char* name, FloatFn fn, int N) {
    float t;
    float mean, stdev, skew, kurt;
    float* vals = malloc(N * sizeof(float));
    if (vals == NULL) return (AlgResults){0};

    uint64_t start_rand_calls = rand15_calls;
    AlgArgs A = {.fn=fn, .arr=vals, .N=N};
    t = time_function(&test_alg, &A);
    uint64_t end_rand_calls = rand15_calls;
    
    mean = get_mean(A.arr, N);
    stdev = get_stdev(A.arr, N);
    skew = get_skewness(A.arr, N);
    kurt = get_kurtosis(A.arr, N);

    double nanoseconds = (double) (t * 1e9 / (double)NUM_VALS);
    double rand_calls = (double) ((double)(end_rand_calls - start_rand_calls) / (double)NUM_VALS);

    AlgResults results = {
        .nickname = {0},
        .nanoseconds = nanoseconds,
        .rand_calls = rand_calls,
        .mean = mean,
        .stdev = stdev,
        .skewness = skew,
        .kurtosis = kurt,
    };

    for (int i = 0; i < NAME_LEN-1 && name[i]; i++) {
        results.nickname[i] = name[i];
    }

    free(vals);
    return results;
}

static void print_alg_results(AlgResults r) {
    printf("%12s      %6.2f  %8.4g   % 10.6f  % 10.6f  % 10.6f  % 10.6f\n",
        r.nickname,
        r.nanoseconds,
        r.rand_calls,
        r.mean,
        r.stdev,
        r.skewness,
        r.kurtosis
    );
}

void print_results_summary(AlgResults *arr, int N) {

    printf("   ALGORITHM     TIME:NS   RNG_PER         MEAN      STDDEV    SKEWNESS    KURTOSIS  \n");
    printf("   ----------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < N; i++) {
        print_alg_results(arr[i]);
    }
    
    printf("   __________________________________________________________________________________\n");
    print_alg_results(get_alg_results("(UNIFORM)", frand, NUM_VALS));
    printf("IDEAL NORMAL          -          -     0.0         1.0         0.0         3.0       \n");
}
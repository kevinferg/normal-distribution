#include <math.h>
#include "stats.h"

double get_mean(double* arr, int N) {
    double sum = 0.0;
    int i;
    for (i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum / ((double) N);
}

double get_stdev(double* arr, int N) {
    return sqrt(get_variance(arr, N));
}

double get_variance(double* arr, int N) {
    double mean = get_mean(arr, N);
    int i;
    double sumsq = 0.0;
    double dev;
    for (i = 0; i < N; i++) {
        dev = mean - arr[i];
        sumsq += dev*dev;
    }
    return sumsq / ((double) N);
}
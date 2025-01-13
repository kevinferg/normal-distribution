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

double get_skewness(double* arr, int N) {
    double mean = get_mean(arr, N);
    double variance = get_variance(arr, N);
    int i;
    double sumcubes = 0.0;
    double dev;
    for (i = 0; i < N; i++) {
        dev = arr[i] - mean;
        sumcubes += dev*dev*dev;
    }
    return (sumcubes / ((double) N)) / pow(variance, 3./2.);
}

double get_kurtosis(double* arr, int N) {
    double mean = get_mean(arr, N);
    double variance = get_variance(arr, N);
    int i;
    double sumfourths = 0.0;
    double dev, devsq;
    for (i = 0; i < N; i++) {
        dev = arr[i] - mean;
        devsq = dev * dev;
        sumfourths += devsq*devsq;
    }
    return (sumfourths / ((double) N)) / (variance*variance);
}
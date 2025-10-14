#include <math.h>
#include "stats.h"

float get_mean(float* arr, int N) {
    float sum = 0.0;
    int i;
    for (i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum / ((float) N);
}

float get_stdev(float* arr, int N) {
    return sqrt(get_variance(arr, N));
}

float get_variance(float* arr, int N) {
    float mean = get_mean(arr, N);
    int i;
    float sumsq = 0.0;
    float dev;
    for (i = 0; i < N; i++) {
        dev = mean - arr[i];
        sumsq += dev*dev;
    }
    return sumsq / ((float) N);
}

float get_skewness(float* arr, int N) {
    float mean = get_mean(arr, N);
    float variance = get_variance(arr, N);
    int i;
    float sumcubes = 0.0;
    float dev;
    for (i = 0; i < N; i++) {
        dev = arr[i] - mean;
        sumcubes += dev*dev*dev;
    }
    return (sumcubes / ((float) N)) / pow(variance, 3./2.);
}

float get_kurtosis(float* arr, int N) {
    float mean = get_mean(arr, N);
    float variance = get_variance(arr, N);
    int i;
    float sumfourths = 0.0;
    float dev, devsq;
    for (i = 0; i < N; i++) {
        dev = arr[i] - mean;
        devsq = dev * dev;
        sumfourths += devsq*devsq;
    }
    return (sumfourths / ((float) N)) / (variance*variance);
}
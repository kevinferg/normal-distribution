#include <math.h>
#include "stats.h"

float get_mean(float* arr, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    return (float)(sum / (double)N);
}

float get_variance(float* arr, int N) {
    double mean = get_mean(arr, N);
    double sumsq = 0.0;
    for (int i = 0; i < N; i++) {
        double dev = arr[i] - mean;
        sumsq += dev * dev;
    }
    return (float)(sumsq / (double)N);
}

float get_stdev(float* arr, int N) {
    return sqrtf(get_variance(arr, N));
}

float get_skewness(float* arr, int N) {
    double mean = get_mean(arr, N);
    double variance = get_variance(arr, N);
    double sumcubes = 0.0;
    for (int i = 0; i < N; i++) {
        double dev = arr[i] - mean;
        sumcubes += dev * dev * dev;
    }
    return (float)((sumcubes / (double)N) / pow(variance, 1.5));
}


float get_kurtosis(float* arr, int N) {
    double mean = get_mean(arr, N);
    double variance = get_variance(arr, N);

    double sumfourths = 0.0;
    for (int i = 0; i < N; i++) {
        double dev = arr[i] - mean;
        double dev2 = dev * dev;
        sumfourths += dev2 * dev2;
    }

    return (float)((sumfourths / (double)N) / (variance * variance));
}

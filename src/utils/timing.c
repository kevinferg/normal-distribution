#include <stdio.h>
#include <time.h>
#include "timing.h"

double time_function(void (*fcn)(void*), void* args) {
    clock_t start, end;
    int status;
    double cpu_time_used;

    start = clock();
    (*fcn)(args);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}
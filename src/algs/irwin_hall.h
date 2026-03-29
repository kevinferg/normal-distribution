#pragma once

#include "uniform.h"

#define IH_TOTAL (12)
#define IH_HALF (IH_TOTAL/2)


float frandn_irwinhall(void) {
    int i;
    float sum = -IH_HALF;
    for (i = 0; i < IH_TOTAL; i++) {
        sum += frand();
    }
    return sum;
}

float frandn_intwinhall(void) {
    int i;
    int32_t sum = -IH_HALF;
    for (i = 0; i < IH_TOTAL; i++) {
        sum += rand15();
    }
    return ((float) (sum)) /((float) (RNG_MAX + 2)) - ((float) IH_HALF);
}
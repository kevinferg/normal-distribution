#pragma once

#include "uniform.h"
#include "utils/probit.h"

float frandn_probit(void) {
    float r;
    do {r = frand();} while (r == 0.0f);
    return probitf(r);
}

float frandn_lookup(void) {
    uint16_t r = rand15();
    return probit_lookup(r);
}

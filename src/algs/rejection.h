#pragma once

#include <math.h>
#define REJECT_BOUND 3.33333f

float frandn_rejection(void) {
    float xrand, prand, pthres;
    do {
        xrand = REJECT_BOUND - 2.0*REJECT_BOUND * frand();
        prand = frand();
        pthres = expf(-0.5*(xrand*xrand));
    } while (prand >= pthres);
    return xrand;
}
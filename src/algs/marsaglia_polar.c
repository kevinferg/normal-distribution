#include <stdint.h>
#include "utils/uniform.h"
#define _USE_MATH_DEFINES
#include <math.h>

float frandn_marsaglia(void) {
    static int has_stored = 0;
    static float stored;
    if (has_stored) {
        has_stored = 0;
        return stored;
    }
    float U, V, C, S;
    do {
        U = frand_open()*2.0f - 1.0f;
        V = frand_open()*2.0f - 1.0f;
        S = U*U + V*V;
    } while (S == 0.0f || S >= 1.0f);
    C = sqrtf(-2.0f*logf(S)/S);
    has_stored = 1;
    stored = C*U;
    return   C*V;
}
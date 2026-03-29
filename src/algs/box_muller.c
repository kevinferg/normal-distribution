#include <stdint.h>
#include "utils/fasttrig.h"
#include "utils/uniform.h"
#define _USE_MATH_DEFINES
#include <math.h>

float frandn_boxmuller(void) {
    static int has_stored = 0;
    static float stored;
    if (has_stored) {
        has_stored = 0;
        return stored;
    }
    float U, V, C;
    U = frand_open();
    V = frand_open();
    C = sqrtf(-2.0f*logf(U));
    has_stored = 1;
    stored = C * cosf(2.0f*(float)M_PI * V);
    return   C * sinf(2.0f*(float)M_PI * V);
}

// Box-Muller with Bhaskara I's rational sin/cos approximations
float frandn_bhaskmuller(void) {
    static int has_stored = 0;
    static float stored;
    if (has_stored) {
        has_stored = 0;
        return stored;
    }
    float U, V, C;
    U = frand_open();
    V = frand_open();
    C = sqrtf(-2.0f*logf(U));
    has_stored = 1;
    stored = C * fastcos(2.0f*(float)M_PI * V);
    return   C * fastsin(2.0f*(float)M_PI * V);
}
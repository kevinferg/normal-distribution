#include "fasttrig.h"

/**********************************************
 *  Bhaskara I's sine approximation formula
 * 
 * See: 
 * https://en.wikipedia.org/wiki/Bh%C4%81skara_I%27s_sine_approximation_formula
 * 
 **********************************************/

#define FASTPI 3.14159265358979323846f

static inline float fast_fmodf(float x, float y) {
    if (y == 0.0f)
        return 0.0f;
    float q = x / y;
    int n = (int) q;
    return x - (float) n * y;
}

static inline float bhaskara(float x) {
    float A, numer, denom;
    A = x * (FASTPI - x);
    numer = 16.0f * A;
    denom = 5.0f*FASTPI*FASTPI - 4.0f*A;
    return numer / denom;
}

float fastsin(float x) {
    int sign = 1;

    if (x < 0.0f) {
        sign *= -1;
        x = -x;
    } // Use symmetry to flip across x=0

    if (x >= 2.0*FASTPI) {
        x = fast_fmodf(x, 2.0f*FASTPI);
    } // fmod to get within [0, 2*pi)

    if (x > FASTPI) {
        sign *= -1;
        x -= FASTPI;
    } // Use symmetry to flip across x=pi

    return sign*bhaskara(x);
}

float fastcos(float x) {
    return fastsin(FASTPI*0.5f - x);
}

// #include <stdio.h>
// int test_fasttrig(void) {
//     double x;
//     for (x = -1.; x <= 7.; x += .25) {
//         printf("\n%5.2f -- sin: %.7f (%.7f)\n"
//                  "         cos: %.7f (%.7f)\n", x, fastsin(x), sin(x), fastcos(x), cos(x));
//     }
//     printf("-----------------------\n");
//     return 0;
// }
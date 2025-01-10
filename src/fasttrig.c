#include "math.h"
#include "fasttrig.h"
#include "stdio.h"

/**********************************************
 *  Bhaskara I's sine approximation formula
 * 
 * See: 
 * https://en.wikipedia.org/wiki/Bh%C4%81skara_I%27s_sine_approximation_formula
 * 
 **********************************************/

static inline float bhaskara(float x) {
    float A, numer, denom;
    A = x * (FASTPI - x);
    numer = 16. * A;
    denom = 5*FASTPI*FASTPI - 4.*A;
    return numer / denom;
}

float fastsin(float x) {
    int sign = 1;

    if (x < 0.) {
        sign *= -1;
        x = -x;
    } // Use symmetry to flip across x=0

    if (x >= 2.0*FASTPI) {
        x = fmodf(x, 2.0*FASTPI);
    } // fmod to get within [0, 2*pi)

    if (x > FASTPI) {
        sign *= -1;
        x -= FASTPI;
    } // Use symmetry to flip across x=pi

    return sign*bhaskara(x);
}

float fastcos(float x) {
    return fastsin(FASTPI*0.5 - x);
}

int test_fasttrig(void) {
    double x;
    for (x = -1.; x <= 7.; x += .25) {
        printf("\n%5.2f -- sin: %.7f (%.7f)\n"
                 "         cos: %.7f (%.7f)\n", x, fastsin(x), sin(x), fastcos(x), cos(x));
    }
    printf("-----------------------\n");
    return 0;
}
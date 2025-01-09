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

double fastsin(double x) {
    double A, numer, denom;
    int sign = 1;

    while (x < 0.)    x += 2.*FASTPI;
    while (x > 2.*FASTPI) x -= 2.*FASTPI;

    if (x > FASTPI) { // Flip sign if x > pi
        x -= FASTPI;
        sign = -1;
    }

    A = x * (FASTPI - x);
    numer = 16. * A;
    denom = 5*FASTPI*FASTPI - 4.*A;
    return sign * numer / denom;
}

double fastcos(double x) {
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
#include "utils/uniform.h"
#include <math.h>

// Improved Ratio Method (Leva 1992)
// https://dl.acm.org/doi/pdf/10.1145/138351.138364
float frandn_ratio2(void) {
    static const float s =  0.449871f;
    static const float t = -0.386595f;
    static const float a =  0.19600f;
    static const float b =  0.25472f;
    float u, v, x, y, Q;
    do {
    u = frand_open();                // u ~ U(0, 1)
    v = 1.7156*(frand_open() - 0.5); // v ~ U(-sqrt(2/e), +sqrt(2/e))

    x = u - s;
    y = fabsf(v) - t;

    Q = x*x + y*(a*y - b*x);
    } while ( Q >= 0.27597 &&      // Instant accept if within inner boundary
             (Q > 0.27846 ||       // Reject if beyond outer boundary OR ...
             v*v > -4*u*u*logf(u)) //    ... if outside acceptance region

             );
    return v/u;
}
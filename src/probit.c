#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "probit.h"

#define PPND16_IMPLEMENTED 0

#define SPLIT_1 0.425f
#define R_CONST 1.6f
#define SPLIT_2 5.0f

#define AB_PREC7_N 4
#define AB_PREC7_HASH 32.3184577772f
#define A_PREC7_0 3.3871327179e0f
#define A_PREC7_1 5.0434271938e1f
#define A_PREC7_2 1.5929113202e2f
#define A_PREC7_3 5.9109374720e1f
#define B_PREC7_0 1.0f
#define B_PREC7_1 1.7895169469e1f
#define B_PREC7_2 7.8757757664e1f
#define B_PREC7_3 6.7187563600e1f

#define CD_PREC7_N 4
#define CD_PREC7_HASH 15.7614929821f
#define C_PREC7_0 1.4234372777e0f
#define C_PREC7_1 2.7568153900e0f
#define C_PREC7_2 1.3067284816e0f
#define C_PREC7_3 1.7023821103e-1f
#define D_PREC7_0 1.0f
#define D_PREC7_1 7.3700164250e-1f
#define D_PREC7_2 1.2021132975e-1f
#define D_PREC7_3 0.0f

#define EF_PREC7_N 4
#define EF_PREC7_HASH 19.4052910204f
#define E_PREC7_0 6.6579051150e0f
#define E_PREC7_1 3.0812263860e0f
#define E_PREC7_2 4.2868294337e-1f
#define E_PREC7_3 1.7337203997e-2f
#define F_PREC7_0 1.0f
#define F_PREC7_1 2.4197894225e-1f
#define F_PREC7_2 1.2258202635e-2f
#define F_PREC7_3 0.0f

/**************** Algorithms *****************/
static inline float R1f(float x) {
    float A = A_PREC7_0;
    float B = B_PREC7_0;

    float xp = x;
    A += A_PREC7_1 * xp;
    B += B_PREC7_1 * xp;

    xp *= x;
    A += A_PREC7_2 * xp;
    B += B_PREC7_2 * xp;

    xp *= x;
    A += A_PREC7_3 * xp;
    B += B_PREC7_3 * xp;

    return A / B;
}

float R2f(float x) {
    float C = C_PREC7_0;
    float D = D_PREC7_0;

    float xp = x;
    C += C_PREC7_1 * xp;
    D += D_PREC7_1 * xp;

    xp *= x;
    C += C_PREC7_2 * xp;
    D += D_PREC7_2 * xp;

    xp *= x;
    C += C_PREC7_3 * xp;
    D += D_PREC7_3 * xp;

    return C / D;
}

float R3f(float x) {
    float E = E_PREC7_0;
    float F = F_PREC7_0;

    float xp = x;
    E += E_PREC7_1 * xp;
    F += F_PREC7_1 * xp;

    xp *= x;
    E += E_PREC7_2 * xp;
    F += F_PREC7_2 * xp;

    xp *= x;
    E += E_PREC7_3 * xp;
    F += F_PREC7_3 * xp;

    return E / F;
}

float probitf(float p) {
    float r, q = p - 0.5;
    if (fabs(q) <= SPLIT_1) {
        return q * R1f(SPLIT_1*SPLIT_1- q*q);
    } else {
        r = sqrtf(-logf(fminf(p, 1-p)));
        if (r <= SPLIT_2) {
            return copysignf(R2f(r-R_CONST), q);
        } else {
            return copysignf(R3f(r-SPLIT_2), q);
        }
    }
}

// float probit_approx(float p) {
//     static const int N = 250;
//     static const float vals[250+1] = {
//         0.000000,0.005013,0.010027,0.015040,0.020054,0.025069,0.030084,0.035100,0.040117,0.045135,0.050154,0.055174,0.060195,0.065219,0.070243,0.075270,
//         0.080298,0.085329,0.090361,0.095396,0.100434,0.105474,0.110516,0.115562,0.120610,0.125661,0.130716,0.135774,0.140835,0.145900,0.150969,0.156042,
//         0.161119,0.166199,0.171285,0.176374,0.181468,0.186567,0.191671,0.196780,0.201893,0.207013,0.212137,0.217267,0.222403,0.227545,0.232693,0.237847,
//         0.243007,0.248174,0.253347,0.258527,0.263714,0.268909,0.274110,0.279319,0.284535,0.289760,0.294992,0.300232,0.305481,0.310738,0.316003,0.321278,
//         0.326561,0.331853,0.337155,0.342466,0.347787,0.353118,0.358459,0.363810,0.369171,0.374544,0.379926,0.385320,0.390726,0.396142,0.401571,0.407011,
//         0.412463,0.417928,0.423405,0.428895,0.434397,0.439913,0.445442,0.450986,0.456542,0.462113,0.467699,0.473299,0.478914,0.484544,0.490189,0.495850,
//         0.501527,0.507221,0.512930,0.518657,0.524400,0.530161,0.535940,0.541736,0.547551,0.553385,0.559237,0.565109,0.571000,0.576910,0.582842,0.588793,
//         0.594766,0.600760,0.606775,0.612813,0.618873,0.624956,0.631062,0.637192,0.643345,0.649524,0.655727,0.661955,0.668209,0.674490,0.680797,0.687131,
//         0.693493,0.699884,0.706302,0.712751,0.719229,0.725737,0.732276,0.738847,0.745450,0.752085,0.758754,0.765456,0.772193,0.778966,0.785774,0.792619,
//         0.799501,0.806421,0.813380,0.820379,0.827418,0.834499,0.841621,0.848787,0.855996,0.863250,0.870550,0.877896,0.885290,0.892733,0.900226,0.907770,
//         0.915365,0.923014,0.930717,0.938476,0.946291,0.954165,0.962099,0.970093,0.978150,0.986271,0.994458,1.002712,1.011034,1.019428,1.027893,1.036433,
//         1.045050,1.053744,1.062519,1.071377,1.080319,1.089349,1.098469,1.107680,1.116987,1.126391,1.135896,1.145505,1.155221,1.165047,1.174987,1.185044,
//         1.195223,1.205527,1.215961,1.226528,1.237235,1.248085,1.259084,1.270237,1.281551,1.293032,1.304685,1.316519,1.328539,1.340755,1.353174,1.365806,
//         1.378659,1.391744,1.405072,1.418654,1.432503,1.446632,1.461056,1.475791,1.490853,1.506262,1.522036,1.538199,1.554773,1.571786,1.589267,1.607247,
//         1.625763,1.644853,1.664563,1.684940,1.706043,1.727934,1.750685,1.774382,1.799118,1.825007,1.852180,1.880794,1.911035,1.943134,1.977369,2.014090,
//         2.053749,2.096928,2.144411,2.197287,2.257128,2.326348,2.408915,2.512145,2.652069,2.878166,nan,
//     };
// }

// double probit(double p) {
//     double r, q = p - 0.5;
//     if (fabs(q) <= SPLIT_1) {
//         return q * R1(SPLIT_1*SPLIT_1- q*q, 16);
//     } else {
//         r = sqrt(-log(fmin(p, 1-p)));
//         if (r <= SPLIT_2) {
//             return copysign(R2(r-R_CONST, 16), q);
//         } else {
//             return copysign(R3(r-SPLIT_2, 16), q);
//         }
//     }
// }

double probit(double p) {
    return probitf(p);
}


/****************  Approximations  ********************/

float inverf(float x) {
    if (fabs(x) >= .975) return copysign(1.95996, x);
    float x2 = x * x, xpow = x, sum = 0.886227 * x;
    xpow *= x2; sum -= 0.654213   * xpow;
    xpow *= x2; sum -= 0.104457   * xpow;
    xpow *= x2; sum -= 0.041004   * xpow;
    xpow *= x2; sum -= 0.0215925  * xpow;
    xpow *= x2; sum -= 0.0132283  * xpow;
    // xpow *= x2; sum -= 0.00889456 * xpow;
    // xpow *= x2; sum -= 0.00637079 * xpow;
    // xpow *= x2; sum -= 0.00477692 * xpow;
    // xpow *= x2; sum -= 0.00370837 * xpow;
    // xpow *= x2; sum -= 0.00295836 * xpow;
    // xpow *= x2; sum -= 0.00241241 * xpow;
    // xpow *= x2; sum -= 0.00200308 * xpow;
    // xpow *= x2; sum -= 0.00168856 * xpow;
    // xpow *= x2; sum -= 0.00144185 * xpow;
    // xpow *= x2; sum -= 0.00124487 * xpow;
    float denominator = 1-x2;
    return sum/denominator;
}

float probit_approx_rational(float y) {
    return inverf((y*2.0-1.0))*1.414213562;
}



static const int num_probit_vals = 250;
static const float probit_vals[251] = {
    0.000000,0.005013,0.010027,0.015040,0.020054,0.025069,0.030084,0.035100,0.040117,0.045135,0.050154,0.055174,0.060195,0.065219,0.070243,0.075270,
    0.080298,0.085329,0.090361,0.095396,0.100434,0.105474,0.110516,0.115562,0.120610,0.125661,0.130716,0.135774,0.140835,0.145900,0.150969,0.156042,
    0.161119,0.166199,0.171285,0.176374,0.181468,0.186567,0.191671,0.196780,0.201893,0.207013,0.212137,0.217267,0.222403,0.227545,0.232693,0.237847,
    0.243007,0.248174,0.253347,0.258527,0.263714,0.268909,0.274110,0.279319,0.284535,0.289760,0.294992,0.300232,0.305481,0.310738,0.316003,0.321278,
    0.326561,0.331853,0.337155,0.342466,0.347787,0.353118,0.358459,0.363810,0.369171,0.374544,0.379926,0.385320,0.390726,0.396142,0.401571,0.407011,
    0.412463,0.417928,0.423405,0.428895,0.434397,0.439913,0.445442,0.450986,0.456542,0.462113,0.467699,0.473299,0.478914,0.484544,0.490189,0.495850,
    0.501527,0.507221,0.512930,0.518657,0.524400,0.530161,0.535940,0.541736,0.547551,0.553385,0.559237,0.565109,0.571000,0.576910,0.582842,0.588793,
    0.594766,0.600760,0.606775,0.612813,0.618873,0.624956,0.631062,0.637192,0.643345,0.649524,0.655727,0.661955,0.668209,0.674490,0.680797,0.687131,
    0.693493,0.699884,0.706302,0.712751,0.719229,0.725737,0.732276,0.738847,0.745450,0.752085,0.758754,0.765456,0.772193,0.778966,0.785774,0.792619,
    0.799501,0.806421,0.813380,0.820379,0.827418,0.834499,0.841621,0.848787,0.855996,0.863250,0.870550,0.877896,0.885290,0.892733,0.900226,0.907770,
    0.915365,0.923014,0.930717,0.938476,0.946291,0.954165,0.962099,0.970093,0.978150,0.986271,0.994458,1.002712,1.011034,1.019428,1.027893,1.036433,
    1.045050,1.053744,1.062519,1.071377,1.080319,1.089349,1.098469,1.107680,1.116987,1.126391,1.135896,1.145505,1.155221,1.165047,1.174987,1.185044,
    1.195223,1.205527,1.215961,1.226528,1.237235,1.248085,1.259084,1.270237,1.281551,1.293032,1.304685,1.316519,1.328539,1.340755,1.353174,1.365806,
    1.378659,1.391744,1.405072,1.418654,1.432503,1.446632,1.461056,1.475791,1.490853,1.506262,1.522036,1.538199,1.554773,1.571786,1.589267,1.607247,
    1.625763,1.644853,1.664563,1.684940,1.706043,1.727934,1.750685,1.774382,1.799118,1.825007,1.852180,1.880794,1.911035,1.943134,1.977369,2.014090,
    2.053749,2.096928,2.144411,2.197287,2.257128,2.326348,2.408915,2.512145,2.652069,2.878166,   3.0
};

// Code to generate the above table:
// for (i = 0; i <= 250; i++) {
//     float x = .5 + i*(.5/250.);
//     printf("%f,", probit7(x));
//     fflush(stdout);
//     if ((i+1)%16 == 0) printf("\n");
// }

float probit_approx_interp(float y) {
    static const float increment = (1.0 - 0.5) / (float) num_probit_vals;
    static const float invincrement = 1./increment;
    float sign = 1.0;
    if (y < .5) {
        sign = -1.0;
        y = 1-y;
    }
    if (y > 1.0) return sign*probit_vals[num_probit_vals];

    int i = (int) ((float) (y - 0.5) * invincrement);
    float w = (y - (0.5 + increment * i))*invincrement;
    float val1 = probit_vals[i];
    float val2 = probit_vals[i+1];
    return sign * (w*val1 + (1-w)*val2);
}

#include "probit_table.h"

float probit_lookup(int index) {
    return probit_table[index];
}

/*********** Test functions ************/

double mantissa10(double x) {
    int less, greater;
    if (x == 0.0) return 0.0;
    double a = fabs(x);
    while ((greater = (a >= 10.)) || (less = (a < 1.))) {
        a *= greater ? 0.1 : 10.;
    }
    return copysign(a, x);
}

int hash_checkf(const float* A, const float* B, double hash, int N, char* name) {
    double sum = 0.0;
    int i, exponent;
    for (i = 0; i < N; i++) {
        sum += mantissa10(A[i]);
        sum += mantissa10(B[i]);
    }
    sum -= 1.0;

    int compare = fabs(sum - hash) <= 1e-6;
    printf("> Hash %s %s\n", name, compare ? "":"               FAIL");
    printf("  Calculated: %12.10f\n", sum);
    printf("    Expected: %12.10f\n", hash);
    printf("  Match? %s\n\n", compare ? "Yes":"No");
    return !compare;
}

#define HASH_PREC7(sum, P) do {        \
    sum += mantissa10(P##_0);          \
    sum += mantissa10(P##_1);          \
    sum += mantissa10(P##_2);          \
    sum += mantissa10(P##_3);          \
} while (0)

#define HASH_PAIR_PREC7(sum, P, Q) do { \
    HASH_PREC7(sum, P);                 \
    HASH_PREC7(sum, Q);                 \
} while (0)


int hash_check_prec7(double hash, char* name, int which) {
    double sum = 0.0;

    if (which == 0) HASH_PAIR_PREC7(sum, A_PREC7, B_PREC7);
    if (which == 1) HASH_PAIR_PREC7(sum, C_PREC7, D_PREC7);
    if (which == 2) HASH_PAIR_PREC7(sum, E_PREC7, F_PREC7);

    sum -= 1.0;

    int compare = fabs(sum - hash) <= 1e-10;
    printf("> Hash %s %s\n", name, compare ? "" : "               FAIL");
    printf("  Calculated: %12.10f\n", sum);
    printf("    Expected: %12.10f\n", hash);
    printf("  Match? %s\n\n", compare ? "Yes" : "No");

    return !compare;
}

int check_probit_vals(void) {
    int i;
    double ps[3] = {0.25, 0.001, 1e-20};
    double vals[3] = { -0.6744897501960817, -3.090232306167814, -9.262340089798408};
    for (i = 0; i < 3; i++) {
        printf("probit(%.2e) = ...\n", ps[i]);
        printf("  Calculated,  7: %0.16f\n", probit7(ps[i]));
        if (PPND16_IMPLEMENTED)
            printf("  Calculated, 16: %0.16f\n", probit16(ps[i]));
        printf("      True Value: %0.16f\n\n", vals[i]);
    }
    return 0;
}

int test_probit(void) {
    printf("---- Coefficient hash checks ----\n\n");

    hash_check_prec7(AB_PREC7_HASH, "AB_PREC7", 0);
    hash_check_prec7(CD_PREC7_HASH, "CD_PREC7", 1);
    hash_check_prec7(EF_PREC7_HASH, "EF_PREC7", 2);

    check_probit_vals();
    return 0;
}
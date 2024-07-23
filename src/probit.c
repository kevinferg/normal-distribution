#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "probit.h"

#define PPND16_IMPLEMENTED 0

#define SPLIT_1 0.425
#define R_CONST 1.6
#define SPLIT_2 5.0

#define AB_PREC7_N 4
#define AB_PREC7_HASH 32.3184577772
const double A_PREC7[AB_PREC7_N] = {
    [0] = 3.3871327179e0,
    [1] = 5.0434271938e1,
    [2] = 1.5929113202e2,
    [3] = 5.9109374720e1,
};
const double B_PREC7[AB_PREC7_N] = {
    [0] = 1.0,
    [1] = 1.7895169469e1,
    [2] = 7.8757757664e1,
    [3] = 6.7187563600e1,
};

#define CD_PREC7_N 4
#define CD_PREC7_HASH 15.7614929821
const double C_PREC7[CD_PREC7_N] = {
    [0] = 1.4234372777e0,
    [1] = 2.7568153900e0,
    [2] = 1.3067284816e0,
    [3] = 1.7023821103e-1,
};
const double D_PREC7[AB_PREC7_N] = {
    [0] = 1.0,
    [1] = 7.3700164250e-1,
    [2] = 1.2021132975e-1,
    [3] = 0.0,
};

#define EF_PREC7_N 4 
#define EF_PREC7_HASH 19.4052910204
const double E_PREC7[EF_PREC7_N] = {
    [0] = 6.6579051150e0,
    [1] = 3.0812263860e0,
    [2] = 4.2868294337e-1,
    [3] = 1.7337203997e-2,
};
const double F_PREC7[EF_PREC7_N] = {
    [0] = 1.0,
    [1] = 2.4197894225e-1,
    [2] = 1.2258202635e-2,
    [3] = 0.0,
};

/**************** Algorithms *****************/
double R1(double x, int precision) {
    int i;
    double A = 0.0, B = 0.0;
    double xp = 1.0;

    switch (precision) {
    case 7:
        for (i = 0; i < AB_PREC7_N; i++) {
            A += A_PREC7[i]*xp;
            B += B_PREC7[i]*xp;
            if (i == AB_PREC7_N-1) return A/B;
            xp *= x;
        }
    case 16: default:
        for (i = 0; i < AB_PREC7_N; i++) {
            A += A_PREC7[i]*xp;
            B += B_PREC7[i]*xp;
            if (i == AB_PREC7_N-1) return A/B;
            xp *= x;
        }


    }
}

double R2(double x, int precision) {
    int i;
    double C = 0.0, D = 0.0;
    double xp = 1.0;

    switch (precision) {
    case 7:
        for (i = 0; i < CD_PREC7_N; i++) {
            C += C_PREC7[i]*xp;
            D += D_PREC7[i]*xp;
            if (i == CD_PREC7_N-1) return C/D;
            xp *= x;
        }
    case 16: default:
        for (i = 0; i < CD_PREC7_N; i++) {
            C += C_PREC7[i]*xp;
            D += D_PREC7[i]*xp;
            if (i == CD_PREC7_N-1) return C/D;
            xp *= x;
        }


    }
}

double R3(double x, int precision) {
    int i;
    double E = 0.0, F = 0.0;
    double xp = 1.0;

    switch (precision) {
    case 7:
        for (i = 0; i < EF_PREC7_N; i++) {
            E += E_PREC7[i]*xp;
            F += F_PREC7[i]*xp;
            if (i == EF_PREC7_N-1) return E/F;
            xp *= x;
        }
    case 16: default:
        for (i = 0; i < EF_PREC7_N; i++) {
            E += E_PREC7[i]*xp;
            F += F_PREC7[i]*xp;
            if (i == EF_PREC7_N-1) return E/F;
            xp *= x;
        }


    }
}

double probit(double p, int precision) {
    double r, q = p - 0.5;
    if (fabs(q) <= SPLIT_1) {
        return q * R1(SPLIT_1*SPLIT_1- q*q, precision);
    } else {
        r = sqrt(-log(fmin(p, 1-p)));
        if (r <= SPLIT_2) {
            return copysign(R2(r-R_CONST, precision), q);
        } else {
            return copysign(R3(r-SPLIT_2, precision), q);
        }
    }
}

double probit7(double p) {
    return probit(p, 7);
}

double probit16(double p) {
    return probit(p, 16);
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

int hash_check(const double* A, const double* B, double hash, int N, char* name) {
    double sum = 0.0;
    int i, exponent;
    for (i = 0; i < N; i++) {
        sum += mantissa10(A[i]);
        sum += mantissa10(B[i]);
    }
    sum -= 1.0;

    int compare = fabs(sum - hash) <= 1e-10;
    printf("> Hash %s %s\n", name, compare ? "":"               FAIL");
    printf("  Calculated: %12.10f\n", sum);
    printf("    Expected: %12.10f\n", hash);
    printf("  Match? %s\n\n", compare ? "Yes":"No");
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
    hash_check(A_PREC7, B_PREC7, AB_PREC7_HASH, AB_PREC7_N, "AB_PREC7");
    hash_check(C_PREC7, D_PREC7, CD_PREC7_HASH, CD_PREC7_N, "CD_PREC7");
    hash_check(E_PREC7, F_PREC7, EF_PREC7_HASH, EF_PREC7_N, "EF_PREC7");

    check_probit_vals();

    return 0;
}

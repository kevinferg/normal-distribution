#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "normal.h"
#include "uniform.h"
#include "probit.h"
#include "fasttrig.h"

#define PI 3.14159265358979323846

const char* alg_names[ALGMAX] = {
    [ACCEPT_REJECT] = "Rejection Sampling",
    [IRWIN_HALL] = "Irwin-Hall",
    [PROB_INT] = "Probability Integral Transform",
    [BOX_MULLER] = "Box-Muller",
    [MARSAGLIA] = "Marsaglia Polar",
    [IRWIN_HALL_INT] = "Irwin-Hall with Integers",
    [BHASKARA_MULLER] = "Box-Muller with Fast Trig",
    [UNWRAP_UNIFORM] = "Unwrapped Uniform",
    [LOOKUP] = "Lookup table",
    [RADEMACHER] = "Weighted Rademacher Sum",
    [COARSE_MEAN] = "Mean of 4 approximate normals",
    [UNIFORM] = "Baseline uniform distribution"
};

const char* alg_names_short[ALGMAX] = {
    [ACCEPT_REJECT] = "rejection",
    [IRWIN_HALL] = "irwin-hall",
    [PROB_INT] = "prob-int",
    [BOX_MULLER] = "box-muller",
    [MARSAGLIA] = "marsaglia",
    [IRWIN_HALL_INT] = "irwin-int",
    [BHASKARA_MULLER] = "bhask-muller",
    [UNWRAP_UNIFORM] = "unwrap-unif",
    [LOOKUP] = "lookup",
    [RADEMACHER] = "rademacher",
    [COARSE_MEAN] = "coarse-mean",
    [UNIFORM] = "(UNIFORM)"
};

const NormalAlgFunction alg_functions[ALGMAX] = {
    [ACCEPT_REJECT] = normal_accept_reject,
    [IRWIN_HALL] = normal_irwin_hall,
    [PROB_INT] = normal_prob_int,
    [BOX_MULLER] = normal_box_muller,
    [MARSAGLIA] = normal_marsaglia,
    [IRWIN_HALL_INT] = normal_irwin_hall_int,
    [BHASKARA_MULLER] = normal_bhaskara_muller,
    [UNWRAP_UNIFORM] = normal_unwrap_uniform,
    [LOOKUP] = normal_lookup,
    [RADEMACHER] = normal_rademacher,
    [COARSE_MEAN] = normal_coarse_mean,
    [UNIFORM] = uniform_multiple
};



/************ Algorithms ************/

#define INV_SQRT_2PI 0.3989422804
#define REJECT_BOUND 3.33333

int normal_accept_reject(double* arr, int N) {
    int i;
    double xrand, prand, pthres;
    for (i = 0; i < N; i++) {
        do {
            xrand = REJECT_BOUND - 2.0*REJECT_BOUND * rand_unif_closed();
            prand = rand_unif_closed();
            pthres = exp(-0.5*(xrand*xrand));
        } while (prand >= pthres);
        arr[i] = xrand;
    }
    return 0;
}


#define IH_TOTAL (12)
#define IH_HALF (IH_TOTAL/2)

int normal_irwin_hall(double* arr, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        arr[i] = 0.0;
        for (j = 0; j < IH_TOTAL; j++) {
            arr[i] += rand_unif_open();
        }
        arr[i] -= IH_HALF;
    }
    return 0;
}

int normal_prob_int(double* arr, int N) {
    int i;
    for (i = 0; i < N; i++) {
        arr[i] = probitf(rand_unif_open());
    }
    return 0;
}

int normal_box_muller(double* arr, int N) {
    int i;
    double U, V, C;
    for (i = 0; i < N; i += 2) {
        U = rand_unif_open();
        V = rand_unif_open();
        C = sqrt(-2.*log(U));
        arr[i]   = C * cos(2*PI * V);
        if (i == N-1) return 0;
        arr[i+1] = C * sin(2*PI * V);
    }
    return 0;
}


int normal_marsaglia(double* arr, int N) {
    int i;
    double U, V, S, C;
    for (i = 0; i < N; i += 2) {
        do {
            U = rand_unif_open()*2.0 - 1;
            V = rand_unif_open()*2.0 - 1;
            S = U*U + V*V;
        } while (S >= 1.0);
        C = sqrt(-2.*log(S)/S);
        arr[i]   = C*U;
        if (i == N-1) return 0;
        arr[i+1] = C*V;
    }
    return 0;
}

int normal_irwin_hall_int(double* arr, int N) {
    int i, j;
    int sum;
    for (i = 0; i < N; i++) {
        sum = IH_HALF;
        for (j = 0; j < IH_TOTAL; j++) {
            sum += rand();
        }
        arr[i] = ((double) (sum)) /((double) (RAND_MAX + 2)) - ((double) IH_HALF);
    }
    return 0;
}

int normal_bhaskara_muller(double* arr, int N) {
    int i;
    double U, V, C;
    for (i = 0; i < N; i += 2) {
        U = rand_unif_open();
        V = rand_unif_open();
        C = sqrt(-2.*log(U));
        arr[i]   = C * fastcos(2*PI * V);
        if (i == N-1) return 0;
        arr[i+1] = C * fastsin(2*PI * V);
    }
    return 0;
}


static double get_unwrapped_normal(double x, double r) {
    static const int ks[10] = {0,-1,1,-2,2,-3,3,-4,4,-5};
    r *= 2.506628275; // sqrt(2*pi)
    int k, i;
    double p = 0;
    double xt;
    for (i = 0; i < 10; i++) {
        k = ks[i];
        xt = x + k;
        p += exp(-0.5*xt*xt);
        if (r < p) break;
    }
    return (x+k);
}

int normal_unwrap_uniform(double* arr, int N) {
    int i;
    double x, r;
    for (i = 0; i < N; i++) {
        x = rand_unif_half_open();
        r = rand_unif_half_open();
        arr[i] = get_unwrapped_normal(x, r);
    }
    return 0;
}


int normal_lookup(double* arr, int N) {
    int i;
    uint16_t r;
    for (i = 0; i < N; i++) {
        r = rand();
        arr[i] = probit_lookup(r);
    }
    return 0;
}




int normal_rademacher(double* arr, int N) {

    /*
    Method:

    w = sqrt((1-a^2)/(1-a^(2n))) = M(1-a)/(1-a^n)
    
    n = 15 // number of bit trials
    M = 3.75 // max value possible
    --> w = .456352;
    --> a = .893825;

    w_i = w * a^i
    x = 50% w_i, 50% -w_i

    // Equivalent algorithm with floats:
    // float w = .3683;
    // float a = .9416;
    // float x = 0;
    // for (int j = 0; j < 15; j++) {
    //     x += (r&1)? w: -w; 
    //     r >>= 1;
    //     w *= a;
    // }
    // arr[i] = x;
    
    */
    int i;
    int x;
    unsigned int r;

    for (i = 0; i < N; i++) {
        r = rand();
        x = -122846;
        x += (r&1) *  24136; r >>= 1;
        x += (r&1) *  22726; r >>= 1;
        x += (r&1) *  21400; r >>= 1;
        x += (r&1) *  20150; r >>= 1;
        x += (r&1) *  18972; r >>= 1;
        x += (r&1) *  17864; r >>= 1;
        x += (r&1) *  16822; r >>= 1;
        x += (r&1) *  15838; r >>= 1;
        x += (r&1) *  14914; r >>= 1;
        x += (r&1) *  14042; r >>= 1;
        x += (r&1) *  13222; r >>= 1;
        x += (r&1) *  12450; r >>= 1;
        x += (r&1) *  11724; r >>= 1;
        x += (r&1) *  11038; r >>= 1;
        x += (r&1) *  10394;

        arr[i] = ((float) x / (float) 32768); 

    }
    return 0;
}

int normal_coarse_mean(double* arr, int N) {
    static const int16_t prob0[16] = {-9555, -5842, -3891, -2666, -1954, -1112, -713, 370, -265, 339, 1480, 2780, 2508, 3849, 6098, 8303};
    static const int16_t prob1[16] = {-8339, -5311, -4676, -2430, -1969, -1431, -101, -284, -225, 968, 1615, 2042, 2668, 4516, 6143, 9003};
    static const int16_t prob2[16] = {-8303, -5924, -3739, -2536, -2945, -1728, -1137, -1282, 339, 1408, 1334, 1334, 3020, 3988, 4477, 8564};
    static const int16_t prob3[8]  = {-6486, -3190, -1834, -801, 1022, 2116, 2882, 6897};

    int i;
    int x;
    unsigned int r;

    for (i = 0; i < N; i++) {
        r = rand();
        x  =  prob0[r&15]; r >>= 4;
        x +=  prob1[r&15]; r >>= 4;
        x +=  prob2[r&15]; r >>= 4;
        x +=  prob3[r&7]; // only 3 of 4 bits are used here, due to rand() limits

        arr[i] = (float) x / (float) (8192.f);
    }
    return 0;


}
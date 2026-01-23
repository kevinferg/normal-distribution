#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "normal.h"
#include "uniform.h"
#include "probit.h"
#include "fasttrig.h"

#define PI_f 3.14159265358979323846f

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

#define INV_SQRT_2PI_f 0.3989422804f
#define REJECT_BOUND 3.33333f

int normal_accept_reject(float* arr, int N) {
    int i;
    float xrand, prand, pthres;
    for (i = 0; i < N; i++) {
        do {
            xrand = REJECT_BOUND - 2.0*REJECT_BOUND * rand_unif_closed();
            prand = rand_unif_closed();
            pthres = expf(-0.5*(xrand*xrand));
        } while (prand >= pthres);
        arr[i] = xrand;
    }
    return 0;
}


#define IH_TOTAL (12)
#define IH_HALF (IH_TOTAL/2)

int normal_irwin_hall(float* arr, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        arr[i] = 0.0f;
        for (j = 0; j < IH_TOTAL; j++) {
            arr[i] += rand_unif_open();
        }
        arr[i] -= IH_HALF;
    }
    return 0;
}

int normal_prob_int(float* arr, int N) {
    int i;
    for (i = 0; i < N; i++) {
        arr[i] = probitf(rand_unif_open());
    }
    return 0;
}

int normal_box_muller(float* arr, int N) {
    int i;
    float U, V, C;
    for (i = 0; i < N; i += 2) {
        U = rand_unif_open();
        V = rand_unif_open();
        C = sqrtf(-2.*logf(U));
        arr[i]   = C * cosf(2.0f*PI_f * V);
        if (i == N-1) return 0;
        arr[i+1] = C * sinf(2.0f*PI_f * V);
    }
    return 0;
}


int normal_marsaglia(float* arr, int N) {
    int i;
    float U, V, S, C;
    for (i = 0; i < N; i += 2) {
        do {
            U = rand_unif_open()*2.0f - 1.0f;
            V = rand_unif_open()*2.0f - 1.0f;
            S = U*U + V*V;
        } while (S >= 1.0f);
        C = sqrtf(-2.0f*logf(S)/S);
        arr[i]   = C*U;
        if (i == N-1) return 0;
        arr[i+1] = C*V;
    }
    return 0;
}

int normal_irwin_hall_int(float* arr, int N) {
    int i, j;
    int sum;
    for (i = 0; i < N; i++) {
        sum = IH_HALF;
        for (j = 0; j < IH_TOTAL; j++) {
            sum += urand();
        }
        arr[i] = ((float) (sum)) /((float) (RAND_MAX + 2)) - ((float) IH_HALF);
    }
    return 0;
}

int normal_bhaskara_muller(float* arr, int N) {
    int i;
    float U, V, C;
    for (i = 0; i < N; i += 2) {
        U = rand_unif_open();
        V = rand_unif_open();
        C = sqrtf(-2.0f*logf(U));
        arr[i]   = C * fastcos(2.0f*PI_f * V);
        if (i == N-1) return 0;
        arr[i+1] = C * fastsin(2.0f*PI_f * V);
    }
    return 0;
}


static float get_unwrapped_normal(float x, float r) {
    static const int8_t ks[10] = {0,-1,1,-2,2,-3,3,-4,4,-5};
    r *= 2.506628275f; // sqrt(2*pi)
    int k, i;
    float p = 0;
    float xt;
    for (i = 0; i < 10; i++) {
        k = ks[i];
        xt = x + k;
        p += expf(-0.5f*xt*xt);
        if (r < p) break;
    }
    return (x+k);
}

int normal_unwrap_uniform(float* arr, int N) {
    int i;
    float x, r;
    for (i = 0; i < N; i++) {
        x = rand_unif_half_open();
        r = rand_unif_half_open();
        arr[i] = get_unwrapped_normal(x, r);
    }
    return 0;
}


int normal_lookup(float* arr, int N) {
    int i;
    uint16_t r;
    for (i = 0; i < N; i++) {
        r = urand();
        arr[i] = probit_lookup(r);
    }
    return 0;
}




int normal_rademacher(float* arr, int N) {

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
        r = urand();
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

int normal_coarse_mean(float* arr, int N) {
    static const int16_t prob0[8] = {14057, -4483, -1017, -3459, 3722, -17266, 9842, -1508};
    static const int16_t prob1[8] = {2529, -134, 12767, -17629, -573, -420, 9000, -4936};
    static const int16_t prob2[8] = {161, -6111, 1506, 4031, -9372, 8232, 5032, 7067};
    static const int16_t prob3[8] = {6952, -4397, 234, -7142, -1974, 4825, -4067, -4181};
    static const int16_t prob4[8]  = {-4936, -4925, -2789, -2348, -9013, 583, 17795, 4346};
    uint32_t i, x;
    uint16_t r;
    for (i = 0; i < N; i++) {
        r = urand();
        x  =  prob0[r&7]; r >>= 3;
        x +=  prob1[r&7]; r >>= 3;
        x +=  prob2[r&7]; r >>= 3;
        x +=  prob3[r&7]; r >>= 3;
        x +=  prob4[r&7];
        arr[i] = (float) x * (1.0f / 16384.0f);
    }
    return 0;
}
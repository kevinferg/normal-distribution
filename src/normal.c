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

static inline float frandn_rademacher(void) {
    static const int16_t w[15] = {
        24136, 22726, 21400, 20150, 18972,
        17864, 16822, 15838, 14914, 14042,
        13222, 12450, 11724, 11038, 10394
    };
    uint16_t r = urand();
    int32_t x = -122846; // -sum(w)/2
    for (int i = 0; i < 15; i++) {
        x += (-(int32_t)((r >> i) & 1)) & w[i];
    } // Ideally the compiler will unroll this.
    return ((float) x) * (1.0f / 32768.0f);
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
    uint32_t i;
    for (i = 0; i < N; i++) {
        arr[i] = frandn_rademacher();
    }
    return 0;
}

static inline float frandn(void) {
    static const int16_t samples[5][8] = {
        {-18666, 1465,  9124, -2456,  7113,   2465,  2440,  -4190},
        { -1690, 1656, -6392,  3616, -7014,   8614,  5799,  -1058},
        { -3021, 4302, -3768, -7831, 11343,  13204, -1676,  -5054},
        {  1012, 6001,  5497, 14892,   487,    967,   -39, -13798},
        { -7858, 3286,  1206, -5579, 14569, -11125, -9127,  -8716}
    }; // ^ Fine-tuned samples from ~N(mu=0, sig=2^14/sqrt(5))
    uint16_t r = urand();           // 15 random bits
    int32_t x;                      // Add 1 entry from each row
    x  = samples[0][r&7];  r >>= 3;
    x += samples[1][r&7];  r >>= 3;
    x += samples[2][r&7];  r >>= 3;
    x += samples[3][r&7];  r >>= 3;
    x += samples[4][r&7];           // Now, x~N(mu=0, sig=2^14)
    return (float) x * (1.0f / 16384.0f);
}

int normal_coarse_mean(float* arr, int N) {
    uint32_t i;
    for (i = 0; i < N; i++) {
        arr[i] = frandn();
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
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
    [NEAR_NORMAL_MEAN] = "Mean of near-normal distributions",
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
    [NEAR_NORMAL_MEAN] = "near-mean",
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
    [NEAR_NORMAL_MEAN] = normal_near_normal_mean,
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


int normal_near_normal_mean(double* arr, int N) {
    int i;
    float x1, x2, x3, x4;
    for (i = 0; i < N; i++) {
        x1 = rand_unif_open();
        x2 = rand_unif_open();
        x3 = rand_unif_open();
        x4 = rand_unif_open();
        x1 = probit_approx_interp(x1);
        x2 = probit_approx_interp(x2);
        x3 = probit_approx_interp(x3);
        x4 = probit_approx_interp(x4);
        arr[i] = (x1+x2+x3+x4)*.5; // .5 = sqrt(1/4)
    }
    return 0;
}

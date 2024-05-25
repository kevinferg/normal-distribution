#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "normal.h"
#include "uniform.h"

#define PI 3.14159265358979323846

const char* alg_names[ALGMAX] = {
    [IRWIN_HALL] = "Irwin-Hall",
    [PROB_INT] = "Probability Integral Transform",
    [BOX_MULLER] = "Box-Muller",
    [MARSAGLIA] = "Marsaglia Polar",
};

const NormalAlgFunction alg_functions[ALGMAX] = {
    [IRWIN_HALL] = normal_irwin_hall,
    [PROB_INT] = normal_prob_int,
    [BOX_MULLER] = normal_box_muller,
    [MARSAGLIA] = normal_marsaglia,
};



/************ Algorithms ************/

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


double probit(double y) {
    // TODO: Seek a better approximation
    return log(y/(1-y)) * sqrt(PI/8.0);
}

int normal_prob_int(double* arr, int N) {
    int i;
    for (i = 0; i < N; i++) {
        arr[i] = probit(rand_unif_open());
    }
    return 0;
}

int normal_box_muller(double* arr, int N) {
    int i;
    double U, V, C;
    if (N < 2) return -1;
    for (i = 0; i < N; i += 2) {
        U = rand_unif_open();
        V = rand_unif_open();
        C = sqrt(-2.*log(U));
        arr[i]   = C * cos(2*PI * V);
        arr[i+1] = C * sin(2*PI * V);
    }
    return 0;
}


int normal_marsaglia(double* arr, int N) {
    int i;
    double U, V, S, C;
    if (N < 2) return -1;
    for (i = 0; i < N; i += 2) {
        do {
            U = rand_unif_open()*2.0 - 1;
            V = rand_unif_open()*2.0 - 1;
            S = U*U + V*V;
        } while (S >= 1.0);
        C = sqrt(-2.*log(S)/S);
        arr[i]   = C*U;
        arr[i+1] = C*V;
    }
    return 0;
}

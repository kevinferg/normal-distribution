#ifndef NORMAL_H
#define NORMAL_H

typedef enum AlgID {
    ACCEPT_REJECT,
    UNWRAP_UNIFORM,
    IRWIN_HALL, 
    IRWIN_HALL_INT,
    BOX_MULLER, 
    MARSAGLIA, 
    BHASKARA_MULLER,
    PROB_INT, 
    RADEMACHER,
    LOOKUP,
    COARSE_MEAN,

    UNIFORM, ALGMAX
} AlgID;

typedef struct AlgArgs {
    AlgID id;
    float* arr;
    int N;
} AlgArgs;

typedef int (*NormalAlgFunction)(float*, int);

extern const char* alg_names[ALGMAX];
extern const char* alg_names_short[ALGMAX];
extern const NormalAlgFunction alg_functions[ALGMAX];

/************ Algorithms ************/

int normal_accept_reject(float* arr, int N);

int normal_irwin_hall(float* arr, int N);

int normal_prob_int(float* arr, int N);

int normal_box_muller(float* arr, int N);

int normal_marsaglia(float* arr, int N);

int normal_irwin_hall_int(float* arr, int N);

int normal_bhaskara_muller(float* arr, int N);

int normal_unwrap_uniform(float* arr, int N);

int normal_lookup(float* arr, int N);

int normal_rademacher(float* arr, int N);

int normal_coarse_mean(float* arr, int N);

#endif
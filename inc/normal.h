#ifndef NORMAL_H
#define NORMAL_H

typedef enum AlgID {
    ACCEPT_REJECT,
    IRWIN_HALL, 
    PROB_INT, 
    BOX_MULLER, 
    MARSAGLIA, 
    IRWIN_HALL_INT,
    BHASKARA_MULLER,
    UNWRAP_UNIFORM,
    NEAR_NORMAL_MEAN,

    UNIFORM, ALGMAX
} AlgID;

typedef struct AlgArgs {
    AlgID id;
    double* arr;
    int N;
} AlgArgs;

typedef int (*NormalAlgFunction)(double*, int);

extern const char* alg_names[ALGMAX];
extern const char* alg_names_short[ALGMAX];
extern const NormalAlgFunction alg_functions[ALGMAX];

/************ Algorithms ************/

int normal_accept_reject(double* arr, int N);

int normal_irwin_hall(double* arr, int N);

int normal_prob_int(double* arr, int N);

int normal_box_muller(double* arr, int N);

int normal_marsaglia(double* arr, int N);

int normal_irwin_hall_int(double* arr, int N);

int normal_bhaskara_muller(double* arr, int N);

int normal_unwrap_uniform(double* arr, int N);

int normal_near_normal_mean(double* arr, int N);

#endif
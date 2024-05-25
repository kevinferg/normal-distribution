#ifndef NORMAL_H
#define NORMAL_H

typedef enum AlgID {
    IRWIN_HALL, 
    PROB_INT, 
    BOX_MULLER, 
    MARSAGLIA, 
    ALGMAX
} AlgID;

typedef struct AlgArgs {
    AlgID id;
    double* arr;
    int N;
} AlgArgs;

typedef int (*NormalAlgFunction)(double*, int);

extern const char* alg_names[ALGMAX];
extern const NormalAlgFunction alg_functions[ALGMAX];

/************ Algorithms ************/

int normal_irwin_hall(double* arr, int N);

int normal_prob_int(double* arr, int N);

int normal_box_muller(double* arr, int N);

int normal_marsaglia(double* arr, int N);



#endif
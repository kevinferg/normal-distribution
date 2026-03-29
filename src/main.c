#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "frandn_tests.h"

/**************************************************************************/
// Get algorithms from alg_list.txt by defining and redefining xmacros

#define ALG_X_PROTOTYPES
#include "algorithms_xmacro.h"
#undef ALG_X_PROTOTYPES

typedef enum {
#define ALG_X_ENUM
#include "algorithms_xmacro.h"
#undef ALG_X_ENUM
    NUM_ALGS
} AlgId;

AlgFunctionInfo alg_table[NUM_ALGS] = {
#define ALG_X_TABLE
#include "algorithms_xmacro.h"
#undef ALG_X_TABLE
};

/**************************************************************************/


int main(int argc, char** argv) {
    int i;
    ALG_ID_boxmuller;
    printf("Generating %d samples using %d algorithms...\n\n", NUM_VALS, NUM_ALGS);
    AlgResults results[NUM_ALGS];
    for (i = 0; i < NUM_ALGS; i++) {
        results[i] = get_alg_results(alg_table[i].nickname, alg_table[i].fn, NUM_VALS);
    }

    sort_results(results, NUM_ALGS);
    print_results_summary(results, NUM_ALGS);

    return 0;
}

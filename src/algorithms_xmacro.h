#if defined(ALG_X_PROTOTYPES)

    #define ADD_ALG(name) float frandn_##name(void);

#elif defined(ALG_X_ENUM)

    #define ADD_ALG(name) ALG_ID_##name,

#elif defined(ALG_X_TABLE)

    #define ADD_ALG(name) \
        [ALG_ID_##name] = { .fn = frandn_##name, .nickname = #name },

#else
    #error "Define one of: ALG_X_PROTOTYPES, ALG_X_ENUM, ALG_X_TABLE"
#endif

#define ALGORITHM_LIST(body) body

#include "alg_list.txt"

#undef ALGORITHM_LIST
#undef ADD_ALG
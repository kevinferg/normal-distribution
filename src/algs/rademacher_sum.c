#include <stdint.h>
#include "utils/uniform.h"

float frandn_rademacher(void) {
    static const int16_t w[15] = {
        24136, 22726, 21400, 20150, 18972,
        17864, 16822, 15838, 14914, 14042,
        13222, 12450, 11724, 11038, 10394
    };
    uint16_t r = rand15();
    int32_t x = -122846; // -sum(w)/2
    for (int i = 0; i < 15; i++) {
        x += (-(int32_t)((r >> i) & 1)) & w[i];
    }
    return ((float) x) * (1.0f / 32768.0f);
}
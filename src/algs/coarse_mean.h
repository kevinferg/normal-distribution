#pragma once

#include <stdint.h>
#include "uniform.h"

float frandn_coarse_mean(void) {
    static const int16_t samples[5][8] = {
        {-18666, 1465,  9124, -2456,  7113,   2465,  2440,  -4190},
        { -1690, 1656, -6392,  3616, -7014,   8614,  5799,  -1058},
        { -3021, 4302, -3768, -7831, 11343,  13204, -1676,  -5054},
        {  1012, 6001,  5497, 14892,   487,    967,   -39, -13798},
        { -7858, 3286,  1206, -5579, 14569, -11125, -9127,  -8716}
    }; // ^ Fine-tuned samples from ~N(mu=0, sig=2^14/sqrt(5))
    uint16_t r = rand15();          // 15 random bits
    int32_t x;                      // Add 1 entry from each row
    x  = samples[0][r&7];  r >>= 3;
    x += samples[1][r&7];  r >>= 3;
    x += samples[2][r&7];  r >>= 3;
    x += samples[3][r&7];  r >>= 3;
    x += samples[4][r&7];           // Now, x~N(mu=0, sig=2^14)
    return (float) x * (1.0f / 16384.0f);
}
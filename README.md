# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution. Includes single- and double-precision implementations of the probit function.

## Results

```
   ALGORITHM     TIME:NS   RNG_PER         MEAN      STDDEV    SKEWNESS    KURTOSIS  
   ----------------------------------------------------------------------------------
   rejection      112.95     5.323    -0.000137    0.994903   -0.000334    2.914604
   boxmuller       38.67         1     0.000173    0.999770   -0.000217    2.999415
 bhaskmuller       24.66         1     0.000057    0.999556    0.000558    2.997025
   marsaglia       19.44     1.273    -0.000252    0.999971   -0.000168    2.999838
   irwinhall       13.79        12     0.000097    1.000006   -0.000251    2.899888
  intwinhall       12.91        12    -0.000809    1.000177   -0.000087    2.900898
      ratio2       12.73     2.737     0.000240    0.999950    0.000035    2.999418
      probit       10.10         1    -0.000474    0.999744    0.000073    2.992119
  rademacher        4.61         1     0.000127    0.999748   -0.000207    2.832666
      lookup        1.98         1     0.000095    0.999596    0.000377    2.991479
  coarsemean        1.95         1    -0.000021    1.000013   -0.000493    2.999223
   __________________________________________________________________________________
   (UNIFORM)        1.56         1     0.500045    0.288661   -0.000117    1.800078
IDEAL NORMAL          -          -     0.0         1.0         0.0         3.0
```

The following algorithm had the fastest performance and maintained good statistics:
```c
/******* Sample a random float from ~N(0, 1) *****/
static inline float frandn(void) {
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
```

## About the Algorithms

### `rejection`: Rejection sampling

1. Sample a random floating point number `x` uniformly on -- in this case, on [-3.33333, 3.33333], covering most outputs of a standard normal sample
2. Sample another float `r` uniformly on [0,1]
3. Evaluate the standard normal pdf at `x`. 
   - If the result exceeds `r`, return `x`
   - Otherwise, start back at (1.)

- Can be thought of as throwing a dart randomly at a rectangular dart board and only accepting darts that land below the standard normal curve. The x-coordinate of the dart is the returned sample.
- More info: [Wikipedia](https://en.wikipedia.org/wiki/Rejection_sampling)


---

### `irwinhall`: Sampling from an Irwin-Hall distribution

1. Generate 12 random floating point numbers, each on [0,1)
2. Add the numbers
3. Subtract 6.0 and return the result

`irwin-int` reduces the number of int-to-float conversions and floating point operations by sampling and adding random integers, and then converting once at the end.

- [Wikipedia](https://en.wikipedia.org/wiki/Irwin%E2%80%93Hall_distribution)

---

### `boxmuller`: Box-Muller Method

This method is based on the fact that 2 independent standard normal random variables `X` and `Y` can be viewed as a vector `[X, Y]` -- This vector has a uniformly random angle `V` on `[0,2*PI)`. Also, the magnitude `C = sqrt(X*X+Y*Y)`, can be transformed into a uniform `[0,1)` random variable as well: `U = exp((-1/2)*C^2)`. The algorithm starts with `U` and `V` and works backwards to compute a corresponding pair `X` and `Y`.  

1. Compute 2 random uniform floats on [0,1): U and V
2. Compute C as `sqrt(-2*log(U))`
3. Return `X = C*cos(2*PI*V)`, and next return `Y = C*sin(2*PI*V)`

`bhaskmuller` uses the much faster Bhaskara I's sine/cosine approximation instead of the built-in sin/cos functions. Accuracy is lower, but this is hardly noticeable, as we are trying to draw random-looking values.

- [Box-Muller Transform](https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform)
- [Bhaskara I's Formula](https://en.wikipedia.org/wiki/Bh%C4%81skara_I%27s_sine_approximation_formula)

---

### `marsaglia`: Marsaglia Polar Method

This algorithm is a slight modification of Box-Muller to avoid using sine/cosine functions. Instead of sampling angle and transformed magnitude as the 2 uniform random values, sample on the box `-1<x<1`, `-1<y<1` and re-sample until the magnitude of the vector is within the unit circle. Now the normally distributed vector `[X,Y]` points in the same direction as this `[x,y]` and rescaling the magnitude is all that is needed to get 2 independent normally distributed numbers `X` and `Y`.

1. Compute 2 random uniform floats on (-1,1): U and V
2. If `S = sqrt(U*U+Y*Y)` exceeds 1, start over at (1.)
3. Transform the vector magnitude using `C = sqrt(-2*log(S)/S)`
4. Return `X = C*U`, and next return `Y = C*V`

- [Wikipedia](https://en.wikipedia.org/wiki/Marsaglia_polar_method)

---

### `probit`: Probability integral transform a.k.a. Inverse transform sampling

If the CDF of a 1-D distribution is known, a uniform random sample can be transformed into a sample of the target distribution by inverting the CDF. That is, you uniformly randomly sample a percentile, and then return the value corresponding to that percentile.

Annoyingly, the CDF for a normal distribution (the 'probit' function) is non-elementary, so some kind of approximation has to be used. I use [Wichura's piecewise polynomial](https://www.jstor.org/stable/2347330) to get ~7 decimal places of accuracy. 

1. Sample a random variable `X~U[0,1)`
2. Return `probit(X)`

- [Inverse Transform Sampling: Wikipedia](https://en.wikipedia.org/wiki/Inverse_transform_sampling)

---

### `rademacher`: Weighted sum of Rademacher trials

A Rademacher random variable can take on the value -1 or 1, each option equally likely. By assigning 15 weights and multiplying them by Rademacher variables (1 for each bit in a `rand()` output), a near-normal distribution can be achieved. I empirically found that choosing weights according to a decreasing geometric sequence gave good control over range and variance of the resulting distribution. First term `.3683` and decay factor `.9416` looked fine. Decay approaching 1 looks better, but more Rademacher variables are needed to avoid artifacts. Lower decay rates lead to worse (i.e. too low) kurtosis. According to [Holzman and Kleitman (1992)](https://link.springer.com/article/10.1007/BF01285819), this process approaches normality as the number of variables increases, so long as weights approach 0. Using pure integer math until the last step saves significant computation time over just using floating point math.

1. Randomly sample a 15-bit integer `R`
2. Set weight `w` value to 0.3683 and initialize `x` to 0
3. Inspect the rightmost bit of `R`. 
   - If 0, subtract `w` from `x`
   - If 1, add `w` to `x`
4. Shift `R` right 1 bit. Multiply `w` by 0.9416
5. Repeat steps (3.) and (4.) for all 15 bits of `R`. Return `x`.

- [Rademacher Distribution: Wikipedia](https://en.wikipedia.org/wiki/Rademacher_distribution)

---

### `lookup`: Lookup table

If you have 131kB memory to spare, precompute probit results for a range of floating point values on (0,1) -- 1 for each possible 15-bit rand() output.

1. Generate a random index from 0 to (RAND_MAX =) 2^15 - 1
2. Return the probit value corresponding to that index

---

### `coarsemean`: Mean of coarse normal samples

Precompute some samples from a standard normal distribution to generate a few short lookup tables. By averaging a few results and scaling them properly, by the central limit theorem the result will be close to normal. Quality of results will depend on the quality of the pregenerated samples. I use 3-bit sections of a rand() call as indices into each table.

1. Generate a random 15-bit number
2. Separate into separate five 3-bit ints
3. For each, look up a normal sample from table with 8 samples per row
4. Add together. Return the appropriately scaled result.

Scaling recommendations:

| rng bits | table size | total entries |
| --- | --- | --- |
| 7 | 3 rows of 2^2 | 12 (1 unused bit) |
| 15 | 5 rows of 2^3 | 40 (0 unused bits) |
| 31 | 7 rows of 2^4 | 112 (3 unused bits) |
| 63 | 12 rows of 2^5 | 384 (3 unused bits) |

Note that int width should also change (e.g. `uint8_t` for 7-bit vs. `uint16_t` for 15-bit).
Also, the unused bits can be used to index into a smaller row if desired.

---

## Todo

- [x] Implement more methods
  - [x] Irwin-Hall with integers
  - [x] Box-Muller with Bhaskara I's sine approximation
  - [x] Rejection sampling
  - [x] Pure lookup table with `RAND_MAX` entries
  - [x] Weighted sum of Rademacher distribution trials
  - [x] Mean of approximate normals
  - [ ] GRAND [(Brent 1973)](https://dl.acm.org/doi/pdf/10.1145/361604.361629)
  - [ ] Ratio method [(Kinderman and Monahan 1977)](https://dl.acm.org/doi/pdf/10.1145/355744.355750)
  - [x] Improved ratio method [(Leva 1992)](https://dl.acm.org/doi/pdf/10.1145/138351.138364)
  - [ ] Direct method [(Wallace 1996)](https://dl.acm.org/doi/pdf/10.1145/225545.225554)
  - [ ] Ziggurat algorithm
- [x] More rigorous statistical testing of results
- [x] Use a random number generator better than rand()
- [x] Probit function improvements
  - [x] Multiple approximation options
  - [x] Implement PPND7 for improved probit approximation
  - [x] Implement PPND16 for even better approximation

## Compilation and Usage
- Windows:
```
mingw32-make
./bin/main.exe
```

- Linux:
```
make
./bin/main
```

See [`doc/README.md`](doc/README.md) for more details.
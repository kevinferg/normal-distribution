# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution. Includes an implementation of PPND7 to compute the probit function.

## Results

```
   ALGORITHM     TIME:NS          MEAN      STDDEV    SKEWNESS    KURTOSIS
   -----------------------------------------------------------------------
   rejection      148.17      0.000620    0.993583    0.000510    2.909401
 unwrap-unif      110.32      0.000014    0.999773   -0.002000    2.994228
  box-muller       26.83      0.000034    0.999922    0.000188    2.997706
bhask-muller       18.22      0.000034    0.999304    0.000192    2.996976
   marsaglia       15.78     -0.000043    0.999944   -0.000461    3.002210
  irwin-hall       13.92     -0.000001    1.000737   -0.004243    2.899020
   irwin-int       12.39     -0.000182    1.000737   -0.004244    2.899011
    prob-int        8.09      0.000002    0.999716   -0.000006    2.991722
  rademacher        4.33      0.000001    0.999729   -0.000001    2.832470
      lookup        2.53      0.000000    0.999718   -0.000001    2.991724
 coarse-mean        1.56     -0.000000    0.999980    0.000005    2.962430
   _______________________________________________________________________
   (UNIFORM)        1.53      0.499984    0.288675    0.000001    1.800000
IDEAL NORMAL          -       0.0         1.0         0.0         3.0     
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

### `unwrap-unif`: 'Unwrapped' uniform distribution

Taking a standard normally distributed random variable Z~N(0,1) and wrapping it onto [0,1),  `Z - floor(Z)`, results in a *very* nearly uniform distribution. (Note: a 12-Irwin-Hall distribution gives exact uniformity.) This algorithm starts with a uniform variable and undoes that wrapping.

1. Start with the wrapped location by sampling a float X~U[0,1)
2. Determine which unit bin it started from by:
   - Sample a random float r on [0,1)
   - Evaluate the pdf at X, then X-1, then X+1, X-2, X+2, etc.
   - When the cumulative pdf exceeds r, you've found the correct bin n
3. Return X + n

---

### `irwin-hall`: Sampling from an Irwin-Hall distribution

1. Generate 12 random floating point numbers, each on [0,1)
2. Add the numbers
3. Subtract 6.0 and return the result

`irwin-int` reduces the number of int-to-float conversions and floating point operations by sampling and adding random integers, and then converting once at the end.

- [Wikipedia](https://en.wikipedia.org/wiki/Irwin%E2%80%93Hall_distribution)

---

### `box-muller`: Box-Muller Method

This method is based on the fact that 2 independent standard normal random variables `X` and `Y` can be viewed as a vector `[X, Y]` -- This vector has a uniformly random angle `V` on `[0,2*PI)`. Also, the magnitude `C = sqrt(X*X+Y*Y)`, can be transformed into a uniform `[0,1)` random variable as well: `U = exp((-1/2)*C^2)`. The algorithm starts with `U` and `V` and works backwards to compute a corresponding pair `X` and `Y`.  

1. Compute 2 random uniform floats on [0,1): U and V
2. Compute C as `sqrt(-2*log(U))`
3. Return `X = C*cos(2*PI*V)`, and next return `Y = C*sin(2*PI*V)`

`bhask-muller` uses the much faster Bhaskara I's sine/cosine approximation instead of the built-in sin/cos functions. Accuracy is lower, but this is hardly noticeable, as we are trying to draw random-looking values.

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

### `prob-int`: Probability integral transform a.k.a. Inverse transform sampling

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

### `coarse-mean`: Mean of 4 coarse normal samples

Precompute some samples from a standard normal distribution to generate a few short lookup tables. By averaging a few results and scaling them properly, by the central limit theorem the result will be close to normal. Quality of results will depend on the quality of the pregenerated samples. I use 4-bit sections of a rand() call as indices into each table.

1. Generate a random 15-bit number
2. Separate into separate 3-4 bit ints: {3, 4, 4, 4}
3. For each, lookup a normal sample from table with {8, 16, 16, 16} samples each
4. Add together. Return the appropriately scaled result.

---

## Todo

- [x] Implement more methods
  - [x] Irwin-Hall with integers
  - [x] Box-Muller with Bhaskara I's sine approximation
  - [x] Rejection sampling
  - [x] Pure lookup table with `RAND_MAX` entries
  - [x] Weighted sum of Rademacher distribution trials
  - [x] Mean of approximate normals
  - [ ] Ziggurat algorithm
- [x] More rigorous statistical testing of results
- [x] Use a random number generator better than rand()
- [ ] Probit function improvements
  - [x] Implement PPND7 for improved probit approximation
  - [ ] Implement PPND16 for even better approximation

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
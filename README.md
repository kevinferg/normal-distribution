# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution. Includes an implementation of PPND7 to compute the probit function.

The currently implemented methods are:
- Rejection Sampling
- Irwin-Hall (+ integer math variant)
- Probability Integral Transform
- Box-Muller (+ fast trig variant)
- Marsaglia Polar
- 'Unwrapped' Uniform Distribution
- Weighted Rademacher Sum
- Lookup Table

Descriptions of some of these can be found on [Wikipedia](https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution).

Todo:
- [x] Implement more methods
  - [x] Irwin-Hall with integers
  - [x] Box-Muller with Bhaskara I's sine approximation
  - [x] Rejection sampling
  - [x] Pure lookup table with `RAND_MAX` entries
  - [x] Weighted sum of Rademacher distribution trials
- [x] More rigorous statistical testing of results
- [ ] Use a random number generator better than rand()
- [ ] Probit function improvements
  - [x] Implement PPND7 for improved probit approximation
  - [ ] Implement PPND16 for even better approximation

## Results

```
   ALGORITHM     TIME:NS          MEAN      STDDEV    SKEWNESS    KURTOSIS
   -----------------------------------------------------------------------
   rejection         967     -0.000447    0.994831   -0.001095    2.913895
  irwin-hall         801      0.000954    1.000047    0.000660    2.898350
    prob-int         151      0.000322    1.000203   -0.001708    2.995847
  box-muller         225      0.000443    0.999773   -0.000006    2.997802
   marsaglia         215     -0.000343    1.000503    0.000538    2.995293
   irwin-int         599      0.000793    1.000017    0.002130    2.898107
bhask-muller         192     -0.000295    0.998977    0.000896    2.996956
 unwrap-unif         757      0.000656    0.999612    0.001184    2.996553
      lookup          72      0.001316    1.000322   -0.002723    2.992055
  rademacher          89     -0.000162    0.999537   -0.001396    2.831396
   _______________________________________________________________________
   (UNIFORM)          65      0.499605    0.288698    0.001400    1.799829
IDEAL NORMAL          -       0.0         1.0         0.0         3.0
```

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
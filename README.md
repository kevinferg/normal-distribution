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
   rejection         962     -0.000447    0.994831   -0.001095    2.913895
  irwin-hall         785      0.000954    1.000047    0.000660    2.898350
 unwrap-unif         755      0.000567    0.999774    0.001360    2.995205
   irwin-int         604      0.000574    1.001000   -0.000931    2.901227
  box-muller         224     -0.000809    1.000156   -0.001216    2.998370
   marsaglia         215     -0.000380    1.000911   -0.002132    3.000197
bhask-muller         197      0.000684    0.999924    0.000625    2.998119
    prob-int         152     -0.000012    0.999587   -0.000055    2.991852
  rademacher          81      0.000220    1.000452    0.001067    2.834682
      lookup          76      0.000076    0.998942   -0.002405    2.987762
 coarse-mean          72     -0.000799    1.066965    0.000837    2.873845
   _______________________________________________________________________
   (UNIFORM)          67      0.500077    0.288752   -0.001075    1.799763
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
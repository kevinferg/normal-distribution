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

Descriptions of these can be found on [Wikipedia](https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution).

Todo:
- [x] Implement more methods
  - [x] Irwin-Hall with integers
  - [x] Box-Muller with Bhaskara I's sine approximation
  - [x] Rejection sampling
- [x] More rigorous statistical testing of results
- [ ] Use a random number generator better than rand()
- [ ] Probit function improvements
  - [x] Implement PPND7 for improved probit approximation
  - [ ] Implement PPND16 for even better approximation

## Results

```
   ALGORITHM     TIME:NS          MEAN      STDDEV    SKEWNESS    KURTOSIS
   -----------------------------------------------------------------------
   rejection         988     -0.000447    0.994831   -0.001095    2.913895
  irwin-hall         781      0.000954    1.000047    0.000660    2.898350
    prob-int         168      0.000322    1.000203   -0.001708    2.995847
  box-muller         233      0.000443    0.999773   -0.000006    2.997802
   marsaglia         215     -0.000343    1.000503    0.000538    2.995293
   irwin-int         608      0.000793    1.000017    0.002130    2.898107
bhask-muller         201     -0.000295    0.998977    0.000896    2.996956
 unwrap-unif         756      0.000656    0.999612    0.001184    2.996553
   near-mean         450      0.000093    0.997186   -0.002742    2.978383
   _______________________________________________________________________
   (UNIFORM)          63      0.499921    0.288663    0.000283    1.800145
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
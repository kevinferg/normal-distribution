# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution. Includes an implementation of PPND7 to compute the probit function.

The currently implemented methods are:
- Rejection Sampling
- Irwin-Hall
- Probability Integral Transform
- Box-Muller
- Marsaglia Polar

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
------------------------------------------------
> "Rejection Sampling" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.000533       Stdev:   0.994992
Skewness:  -0.000673    Kurtosis:   2.914969
Time (ms): 1601
------------------------------------------------

------------------------------------------------
> "Irwin-Hall" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000376       Stdev:   1.000109
Skewness:   0.000048    Kurtosis:   2.899250
Time (ms): 1195
------------------------------------------------

------------------------------------------------
> "Probability Integral Transform" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000096       Stdev:   0.999577
Skewness:  -0.001203    Kurtosis:   2.988304
Time (ms): 295
------------------------------------------------

------------------------------------------------
> "Box-Muller" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.001156       Stdev:   1.000016
Skewness:   0.000478    Kurtosis:   2.996292
Time (ms): 748
------------------------------------------------

------------------------------------------------
> "Marsaglia Polar" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000257       Stdev:   1.000170
Skewness:   0.000639    Kurtosis:   2.999720
Time (ms): 424
------------------------------------------------

------------------------------------------------
> "Irwin-Hall with Integers" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.000649       Stdev:   1.000055
Skewness:   0.000587    Kurtosis:   2.896108
Time (ms): 1013
------------------------------------------------

------------------------------------------------
> "Box-Muller with Fast Trig" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.000338       Stdev:   0.999357
Skewness:   0.000975    Kurtosis:   2.995304
Time (ms): 481
------------------------------------------------
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
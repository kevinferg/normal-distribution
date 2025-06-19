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
> Sampling 3276700 numbers from N(0,1)
    Mean:  -0.000447       Stdev:   0.994831
Skewness:  -0.001095    Kurtosis:   2.913895
Time (ms): 3210
------------------------------------------------

------------------------------------------------
> "Irwin-Hall" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:   0.000954       Stdev:   1.000047
Skewness:   0.000660    Kurtosis:   2.898350
Time (ms): 2648
------------------------------------------------

------------------------------------------------
> "Probability Integral Transform" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:   0.000322       Stdev:   1.000203
Skewness:  -0.001708    Kurtosis:   2.995847
Time (ms): 508
------------------------------------------------

------------------------------------------------
> "Box-Muller" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:   0.000443       Stdev:   0.999773
Skewness:  -0.000006    Kurtosis:   2.997802
Time (ms): 721
------------------------------------------------

------------------------------------------------
> "Marsaglia Polar" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:  -0.000343       Stdev:   1.000503
Skewness:   0.000538    Kurtosis:   2.995293
Time (ms): 699
------------------------------------------------

------------------------------------------------
> "Irwin-Hall with Integers" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:   0.000793       Stdev:   1.000017
Skewness:   0.002130    Kurtosis:   2.898107
Time (ms): 1921
------------------------------------------------

------------------------------------------------
> "Box-Muller with Fast Trig" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:  -0.000295       Stdev:   0.998977
Skewness:   0.000896    Kurtosis:   2.996956
Time (ms): 622
------------------------------------------------

------------------------------------------------
> "Unwrapped Uniform" Method
> Sampling 3276700 numbers from N(0,1)
    Mean:   0.000656       Stdev:   0.999612
Skewness:   0.001184    Kurtosis:   2.996553
Time (ms): 2454
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
# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution. Includes an implementation of PPND7 to compute the probit function.

The currently implemented methods are:
- Irwin-Hall
- Probability Integral Transform
- Box-Muller
- Marsaglia Polar

Descriptions of these can be found on [Wikipedia](https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution).

Todo:
- [ ] Implement more methods
  - [x] Irwin-Hall with integers
  - [x] Box-Muller with Bhaskara I's sine approximation
- [x] More rigorous statistical testing of results
- [ ] Use a random number generator better than rand()
- [ ] Probit function improvements
  - [x] Implement PPND7 for improved probit approximation
  - [ ] Implement PPND16 for even better approximation

## Results

```
------------------------------------------------
> "Irwin-Hall" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000643       Stdev:   1.000260
Skewness:   0.000101    Kurtosis:   2.900275
Time (ms): 1211
------------------------------------------------

------------------------------------------------
> "Probability Integral Transform" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.000382       Stdev:   0.999696
Skewness:  -0.001391    Kurtosis:   2.989751
Time (ms): 565
------------------------------------------------

------------------------------------------------
> "Box-Muller" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000096       Stdev:   1.000082
Skewness:   0.000935    Kurtosis:   2.997576
Time (ms): 707
------------------------------------------------

------------------------------------------------
> "Marsaglia Polar" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000009       Stdev:   1.000022
Skewness:   0.000136    Kurtosis:   2.998573
Time (ms): 456
------------------------------------------------

------------------------------------------------
> "Irwin-Hall with Integers" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:  -0.000202       Stdev:   1.000174
Skewness:   0.000569    Kurtosis:   2.898452
Time (ms): 1001
------------------------------------------------

------------------------------------------------
> "Box-Muller with Fast Trig" Method
> Sampling 10000000 numbers from N(0,1)
    Mean:   0.000255       Stdev:   0.999460
Skewness:  -0.001030    Kurtosis:   2.995563
Time (ms): 474
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
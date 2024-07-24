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
- [ ] More rigorous statistical testing of results
- [ ] Use a random number generator better than rand()
- [ ] Probit function improvements
  - [x] Implement PPND7 for improved probit approximation
  - [ ] Implement PPND16 for even better approximation

## Results

```
------------------------------------------------
> "Irwin-Hall" Method
> Sampling 10000000 numbers from N(0,1)
Mean:   0.000643
Stdev:  1.000260
Time (ms): 9326
------------------------------------------------

------------------------------------------------
> "Probability Integral Transform" Method
> Sampling 10000000 numbers from N(0,1)
Mean:  -0.000382
Stdev:  0.999696
Time (ms): 3320
------------------------------------------------

------------------------------------------------
> "Box-Muller" Method
> Sampling 10000000 numbers from N(0,1)
Mean:   0.000096
Stdev:  1.000082
Time (ms): 4975
------------------------------------------------

------------------------------------------------
> "Marsaglia Polar" Method
> Sampling 10000000 numbers from N(0,1)
Mean:   0.000009
Stdev:  1.000022
Time (ms): 3057
------------------------------------------------

------------------------------------------------
> "Irwin-Hall with Integers" Method
> Sampling 10000000 numbers from N(0,1)
Mean:  -0.000202
Stdev:  1.000174
Time (ms): 6995
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
./bin/main.exe
```

See [`doc/README.md`](doc/README.md) for more details.
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
- [ ] More rigorous statistical testing of results
- [ ] Use a random number generator better than rand()
- [x] Implement PPND7 for improved probit approximation
- [ ] Further mprove probit() approximation to use PPND16

## Results

```
------------------------------------------------
> "Irwin-Hall" Method
> Sampling 1000000 numbers from N(0,1)
Mean:  -0.000283
Stdev:  0.999789
Time (ms): 891
------------------------------------------------

------------------------------------------------
> "Probability Integral Transform" Method
> Sampling 1000000 numbers from N(0,1)
Mean:   0.000138
Stdev:  1.000880
Time (ms): 331
------------------------------------------------

------------------------------------------------
> "Box-Muller" Method
> Sampling 1000000 numbers from N(0,1)
Mean:   0.000483
Stdev:  0.998407
Time (ms): 488
------------------------------------------------

------------------------------------------------
> "Marsaglia Polar" Method
> Sampling 1000000 numbers from N(0,1)
Mean:  -0.000676
Stdev:  1.000444
Time (ms): 328
------------------------------------------------
```

## Compiling
On Windows with MinGW installed, compile by running the following from the main directory:
```
mingw32-make
```
After compiling, the executable will be located in `bin/`

---
See the [`doc/README.md`](doc/README.md) for more details, such as how to compile on other systems.
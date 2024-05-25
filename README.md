# Sampling from a standard normal distribution

## Description
Comparing several algorithms for sampling from a normal distribution.  

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
- [ ] Improve probit() approximation in PIT method

## Results

```
-----------------------------------------
> Method "Irwin-Hall"
Sampling 1000000 numbers
Mean: -0.000283
Standard deviation: 0.999789
Time (ms): 0.973000
-----------------------------------------

-----------------------------------------
> Method "Probability Integral Transform"
Sampling 1000000 numbers
Mean: 0.000270
Standard deviation: 1.137532
Time (ms): 0.341000
-----------------------------------------

-----------------------------------------
> Method "Box-Muller"
Sampling 1000000 numbers
Mean: 0.000483
Standard deviation: 0.998407
Time (ms): 0.498000
-----------------------------------------

-----------------------------------------
> Method "Marsaglia Polar"
Sampling 1000000 numbers
Mean: -0.000676
Standard deviation: 1.000444
Time (ms): 0.320000
-----------------------------------------
```

## Compiling
On Windows with MinGW installed, compile by running the following from the main directory:
```
mingw32-make
```
After compiling, the executable will be located in `bin/`

---
See the [`doc/README.md`](doc/README.md) for more details, such as how to compile on other systems.
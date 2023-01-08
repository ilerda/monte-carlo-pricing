# Monte Carlo Pricing Simulation in C++

## Aim

This is an investigation of how performance of C++ compares to performance of Python in relation to Monte Carlo Simulations.

It follows an example in Hilpisch's *Python for Finance* where the improved performance of Numpy over pure Python is demonstrated.

The aim is to run a Monte Carlo simulation with 50 time steps for pricing a European Stock Option 250000 times to estimate the value after a year.

The method is derived from the Black-Scholes-Merton model where the option's underlying risk factor follows a geometric Brownian motion:

$S_{T} = S_{0} exp((r-\frac{1}{2}\sigma^2)T + \sigma\sqrt{T}z)$

where:

- $S_{0}$ is the initial Stock Index Level
- $r$ is the riskless Short Rate
- $\sigma$ is the volatility
- $T$ is the Time to maturity
- $z$ is a standard normally distributed random variable.


## Methods

### Python

The first Python script `monte_carlo_slow.py` uses a nested Python for loop and a growing 2-D list calculate the result.

The second Python script `monte_carlo_fast.py` only uses vectorised numpy methods with no for loops.

This is achieved by employing the log version of the Euler discretisation of the stochastic differential equation. This means it can be done in a vectorised way because it is completely additive.

Both Python scripts are taken verbatim from *Python for Finance*.

### C++

The C++ function `monte_carlo_pricing()` in `monte.cpp` is implemented with the same nested for loop as the slow Python script.

But it does pre-allocate a 2-D vector to hold the results instead of dynamically appending to a list array like in the Python example.

The second C++ function `monte_carlo_pricing_fast()` is more optimised. This method does not create an array to hold the paths of all the data points but only calculates the final result. It also uses the log version of the Euler discretisation.

In both C++ methods I use a gnu compiler with the optimisations turned on fully by providing a `-O3` flag. That means in order to compile, I would run:

`g++ -o montecarlo -O3 --std=c++17 main.cpp monte.cpp`

and then run:

`./montecarlo`

to execute the program.

## Results

- The slow Python method takes 7.889 seconds and returns a price of 7.999.
- The fast Python method takes 0.4052 seconds and returns a price of 8.1658.
- The slow C++ method takes 0.4132 seconds and returns a price of 8.0296.
- The fast C++ method takes 0.1466 seconds and returns a price of 8.1154.

As we can see, using C++ in an optimised way leads to much better performance than using Python. Even when using vectorisation in Python through the help of Numpy, the performance is only marginally better than the slower implementation of the method in C++.

For a simple academic example like this we can get away with using Python in combination with Numpy, since the implementation is easier to understand and the performance difference is unlikely to matter. However, in a live, large scale production system where lots of data is ingested we should always prefer C++ over Python.

## Further Thoughts

A sometimes overlooked factor in performance considerations is space complexity. Both Python implementations and the slower C++ implementation retain an array of data points of all the simulations. This can be useful if we want to visualise the paths that the Monte Carlo Simulations take.

The following is a visualisation the first ten paths as calculated by the slower C++ method: [Ten Simulation Paths](https://github.com/ilerda/monte-carlo-pricing/blob/main/simulated_prices.png)

In the fast C++ implementation, we do not retain the full array of data points, but only calculate the final result. This saves about 50 MB of space since one float takes up 4 bytes, and there are 250000 times 50 elements in the 2D vector. That is not taking into account any overhead.

Space will become an issue in any real-life production system, so it is best to optimise for space as well when designing systems at a large scale. C++ is very well suited for this.

## Reference

*Python for Finance* by Yves Hilpisch (O’Reilly). Copyright 2015 Yves Hilpisch

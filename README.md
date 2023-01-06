## Monte Carlo Pricing Simulation in C++

#### Aim

This is an investigation of how performance of C++ compares to performance of Python in relation to Monte Carlo Simulations.

It follows an example in Hilpisch's *Python for Finance* where the improved performance of Numpy over pure Python is demonstrated.

The aim is to run a Monte Carlo simulation with 50 time steps for pricing a European Stock Option 250000 times to estimate the value after a year.

#### Results

The first Python script `monte_carlo_slow.py` uses a nested Python for loop and a growing 2-D list to do this.

On my machine, it takes 7.777 seconds and returns a price of 7.999.

The same method is implemented with the same for loop in the C++ function `monte_carlo_pricing()` in `monte.cpp`.

But it does pre-allocate a 2-D vector to hold the results instead of dynamically appending to a list array like in the Python example.

This takes only 1.10722 seconds and returns a price of 8.0311.

The second Python script `monte_carlo_fast.py` only uses vectorised numpy methods with no for loops.

It is achieved by employing the log version of the Euler discretisation of the SDE. This means it can be done in a vectorised way because it is completely additive.

This takes 0.4152 seconds and returns a result of 8.1658.

#### Reference

*Python for Finance* by Yves Hilpisch (O’Reilly). Copyright 2015 Yves Hilpisch

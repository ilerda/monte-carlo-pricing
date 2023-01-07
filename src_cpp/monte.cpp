#include "monte.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

std::vector<std::vector<float>> monte_carlo_pricing()
{
    // measure runtime
    auto begin {std::chrono::high_resolution_clock::now()};

    // time steps
    constexpr size_t M {50};
    // time interval
    float dt {g_T / M};
    // number of paths
    constexpr size_t I {250000};

    // generator for normally distributed numbers.
    std::default_random_engine generator(time(0));
    std::normal_distribution<float> distribution(0.0, 1.0);

    float estimator {};

    // 2-D vector containing the outcomes of all time-steps over all simulations.
    std::vector<std::vector<float>> arr (I, std::vector<float> (M+1));

    for (size_t i {0}; i<I; ++i)
    {
        // Outer loop: go through every simulation we want to run.
        for (size_t t {0}; t<=M; ++t)
        {
            // Inner loop: go through every time step.
            if (t == 0)
                arr[i][t] = g_S0;
            else
            {
                // normally distributed random variable
                float z {distribution(generator)};
                // Euler discretisation of Black-Scholes-Merton SDE
                arr[i][t] = arr[i][t - 1] * exp(
                    (g_r - 0.5 * pow(g_sigma, 2.0)) * dt + g_sigma * sqrt(dt) * z);
            }
        }
        // Sum the result of each simulation minus the strike price, if it is greater than zero.
        if ((arr[i][50] - g_K) > 0.0)
        {
            estimator += arr[i][50] - g_K;
        }
    }
    // Calculating the overall Monte Carlo estimator
    estimator = exp(-g_r * g_T) * estimator / I;

    // Calculating total runtime.
    auto end {std::chrono::high_resolution_clock::now()};
    auto elapsed {std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)};

    std::cout << "The European option value is: " << estimator << '\n';
    std::cout << "The execution time was: " << elapsed.count() * 1e-9 << " seconds.\n";

    return arr;
}

float monte_carlo_pricing_fast()
{
    // Here, we only calculate the final option price and do not retain the array of data points
    // in order to optimise for space.

    // measure runtime
    auto begin {std::chrono::high_resolution_clock::now()};

    // time steps
    constexpr size_t M {50};
    // time interval
    float dt {g_T / M};
    // number of paths
    constexpr size_t I {250000};

    // generator for normally distributed numbers.
    std::default_random_engine generator(time(0));
    std::normal_distribution<float> distribution(0.0, 1.0);

    float estimator {};

    for (size_t i {0}; i<I; ++i)
    {
        // Outer loop: go through every simulation we want to run.
        float new_sum {0.0};
        for (size_t t {0}; t<=M; ++t)
        {
            float z {distribution(generator)};
            // Calculate the Log Version of Euler discretisation of Black-Scholes-Merton SDE
            new_sum += (g_r - 0.5 * pow(g_sigma, 2.0)) * dt + g_sigma * sqrt(dt) * z;
        }

        float result {g_S0 * exp(new_sum)};
        // Sum the result of each simulation minus the strike price, if it is greater than zero.
        if ((result - g_K) > 0.0)
        {
            estimator += result - g_K;
        }
    }
    // Calculating the overall Monte Carlo estimator
    estimator = exp(-g_r * g_T) * estimator / I;

    // Calculating total runtime.
    auto end {std::chrono::high_resolution_clock::now()};
    auto elapsed {std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)};

    std::cout << "The European option value is: " << estimator << '\n';
    std::cout << "The fast execution time was: " << elapsed.count() * 1e-9 << " seconds.\n";

    return estimator;
}

void generate_csv(std::vector<std::vector<float>>& arr)
{
    // Write the first ten simulated paths to a csv file called "paths.csv".
    // Takes the vector returned by monte_carlo_pricing() as a parameter.
    std::ofstream out("paths.csv");

    for (size_t i {0}; i < 10; ++i)
    {
        for (auto col : arr[i])
        {
            out << col << ',';
        }
        out << '\n';
    }
}

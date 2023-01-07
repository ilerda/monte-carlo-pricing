#ifndef MONTE_H
#define MONTE_H

#include <vector>

// initial stock price
constexpr float g_S0 {100.0};
// strike price of call option
constexpr float g_K {105.0};
// time to maturity (year)
constexpr float g_T {1.0};
// constant, riskless short rate
constexpr float g_r {0.05};
// constant volatility
constexpr float g_sigma {0.2};

std::vector<std::vector<float>> monte_carlo_pricing();
float monte_carlo_pricing_fast();
void generate_csv(std::vector<std::vector<float>>& arr);

#endif

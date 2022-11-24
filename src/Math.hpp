#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <complex>
#include <vector>

// This allows us to conviently write complex numbers (i.e 4.0 + 1i)
using namespace std::complex_literals;

// PI constant
const double PI = 4.0 * atan(1);

// Calculates floor(log2(a))
int integerLog2(int a);

// Calculates a^b using repeated squaring
long long integerPower(int a, int b);

// Calculates a^b mod n using repeated squaring
int integerPowerMod(int a, long long b, int n);

// Calculates gcd(a, b) using Euclidean algorithm
long long gcd(long long a, long long b);

std::vector<int> continuedFractionExpansion(int a, int b);

std::pair<int, int> fractionFromExpansion(const std::vector<int>& expansion);

#endif

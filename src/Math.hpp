#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <complex>
#include <vector>
#include <utility>

// This allows us to conviently write complex numbers (i.e 4.0 + 1i)
using namespace std::complex_literals;

// PI constant
const double PI = 4.0 * atan(1);

// Calculates floor(log2(a))
int integerLog2(int a);

// Calculates a^b mod n using repeated squaring
int integerPowerMod(int a, long long b, int n);

// Calculates gcd(a, b) using Euclidean algorithm
long long gcd(long long a, long long b);

// Given a fraction, calculate its continued fraction expansion (used in Shor's algorithm)
std::vector<int> continuedFractionExpansion(int a, int b);

// Given a continued fraction expansion, recover the actual fraction (used in Shor's algorithm)
std::pair<int, int> fractionFromExpansion(const std::vector<int>& expansion);

#endif

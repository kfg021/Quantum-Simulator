#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <complex>

// This allows us to conviently write complex numbers (i.e 4.0 + 1i)
using namespace std::complex_literals;

const double PI = 4.0 * atan(1);

int integerLog2(int x);
int gcd(int a, int b);

#endif

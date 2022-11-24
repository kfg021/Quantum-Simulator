#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Unitary.hpp"
#include "Function.hpp"
#include <string>

enum DeutschJozsaResult { 
    CONSTANT, BALANCED 
};

/* 
Given a function f with domain of size N that is either constant (evaluates to the save value for every input) or balanced (half of f's outputs are 0 and the other half are 1),
this algorithm will determine which type of function f is.
This algorithm can find the answer with only one function evaluation (the best classical algorithm requires O(2^N) evaluations).

For the Deutsch-Jozsa algorithm to work f must satisfy:
    * f(x) = 0 or 1 for all integers 0 <= x < 2^n. (N = 2^n for some positive integer n)
    * f(x) must either be CONSTANT (f(x) = a for all values of x) or BALANCED (f(x) = 0 for exactly half of the x in its domain, and equal to 1 for the other half)

This function is given to the algorithm in the form of a unitary oracle satisfying oracle(|x>|y>) = |x>|y xor f(x)>.
This oracle can be generated from a valid function f using the makeBitOracle function defined below.
*/
DeutschJozsaResult DeutschJozsa(const Bijection& oracle);

/*
Constructs a bit oracle given a valid function f with range = {0, 1} for use in the Deutsch-Jozsa algorithm.
The oracle is returned in the form of a bijection of size 2^n, where oracle(|x>|y>) = |x>|y xor f(x)>.
This function assumes that f is given as a boolean vector of size 2^n for some positive integer n, where f[x] is f evaluated at x.
*/
Bijection makeBitOracle(const std::vector<bool>& f);

/*
Given a function f(x) with domain of size N evaluating 1 for m values of x, this algorithm finds some x where f(x) = 1.
This is a quantum search algorithm that can find an answer in O(sqrt(N)) function evaluations (the best classical algorithm requires O(N) evaluations).

For Grover's algorithm to work f must satisfy: 
    * f(x) = 0 or 1 for all integers 0 <= x < N. (N = 2^n for some positive integer n)
    * f(x) = 1 for m distinct values of x

Similar to Deutsch-Jozsa, the function is also given in the form of an oracle. However, the form of the oracle is slightly different.
The oracle for Grover's algorithm is a phase oracle that must satisfy oracle(|x>) = (-1)^f(x) |x>, and can be generated using the makePhaseOracle function below.

For the algorithm to work, we need to provide m (the number of possible answers) as input.
There is also a more complicated version of Grover's algorithm that works without having to know the number of answers, but that is not implemented here.
*/
int Grover(const Rotation& oracle, int numAnswers);

/*
Constructs a phase oracle given a valid function f with range = {0, 1} for use in Grover's algorithm.
The oracle is returned in the form of a rotation function of size 2^n, where oracle(|x>) = (-1)^f(x) |x>.
This function assumes that f is given as a boolean vector of size 2^n for some positive integer n, where f[x] is f evaluated at x.
*/
Rotation makePhaseOracle(const std::vector<bool>& f);

std::pair<int, int> Shor(int N);

std::pair<int, int> Shor(int N, int a);

#endif
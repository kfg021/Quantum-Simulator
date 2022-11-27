#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Unitary.hpp"
#include "Function.hpp"
#include "QuantumRegister.hpp"

// TODO: consider using the same oracle for grovers algorithm and constructing a phase oracle from the bit oracle.

/*
Return type for the Deutsch-Jozsa algorithm (defined below)
*/
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

int Simon(const Bijection& oracle);

/*
Constructs a bit oracle given a valid function f for use in the Deutsch-Jozsa and Simon algorithms.
The oracle is returned in the form of a bijection of size 2^n, where oracle(|x>|y>) = |x>|y xor f(x)>.
This function assumes that f is given as a vector of size 2^n for some positive integer n, where f[x] is f evaluated at x.
We also need to provide f's output size (in bits). For the Deutsch-Jozsa algorithm it is 1 (since f can only return 0 or 1),
and in Simon's algorithm it is n (since f can return any positive integer less than 2^n).
*/
Bijection makeBitOracle(const std::vector<int>& f, int outputSize);

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

/*
Computes the quantum Fourier transform (QFT) of a section of a quantum register.
The QFT takes the quantum state |j> to the state 1/sqrt(N) sum(k=0 to N-1) exp(2 pi i j k / N).
This implementation constructs the QFT using only one- and two-qubit gates.
*/
void QFT(QuantumRegister& qr, int start, int end);

/*
Computes the inverse quantum Fourier transform (IQFT) of a section of a quantum register.
The IQFT takes the quantum state |j> to the state 1/sqrt(N) sum(k=0 to N-1) exp(-2 pi i j k / N).
This implementation constructs the QFT using only one- and two-qubit gates.
*/
void IQFT(QuantumRegister& qr, int start, int end);

/*
Return type for Shor's algorithm (defined below)
*/
struct ShorResult {
    int factor1;
    int factor2;
    bool operator==(const ShorResult& sr){
        return factor1 == sr.factor1 && factor2 == sr.factor2;
    }
};
const ShorResult SHOR_INVALID = {-1, -1};

/*
Given an integer N that is the product of two primes, calculate its factors.
Shor's algorithm works by picking a random number a less than N, finding the period of the function f(x) = a^x mod N,
and using this period to construct much better guesses for the factors of N.
On real quantum hardware, Shor's algorithm runs in polynomial time.

Since this function can take a while (we may need to run the quantum subroutine multiple times and quantum simulation takes exponential time on classical hardware),
there is also an option to log progress updates.
*/
ShorResult Shor(int N, bool log = false);

/*
This is verion of Shor's algorithm where the guess a is given. This function runs the quantum subroutine once, returns the factors of N if it finds them,
and otherwise returns SHOR_INVALID.
*/
ShorResult Shor(int N, int a, bool log = false);

#endif
#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <complex>

/*
Represents a bijection, that is, a one-to-one mapping.
We could instead represent this as a unitary matrix, but the matrix would have O(n^2) zeros,
so we can save computation time by only storing a mapping of size O(n).
*/
class Bijection{
    private:
    const std::vector<int> f;

    public:
    Bijection(const std::vector<int>& f);
    int size() const;
    int apply(int x) const;
    Bijection controlled() const;
};

/*
Represents a rotation, which multiplies each qubit by some complex number of magnitude 1.
Similar to the bijection, we could instead represent this as a unitary matrix, but the mtrix would have O(n^2) zeros,
so we can save computation time by only storing a mapping of size O(n).
*/
class Rotation{
    private:
    const std::vector<std::complex<double>> f;

    public:
    Rotation(const std::vector<std::complex<double>>& f);
    int size() const;
    std::complex<double> getRotation(int x) const;
    Rotation controlled() const;
};

#endif
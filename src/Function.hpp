#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>
#include <complex>

class Bijection{
    private:
    const std::vector<int> f;

    public:
    Bijection(const std::vector<int>& f);
    int size() const;
    int apply(int x) const;
    Bijection controlled() const;
};

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
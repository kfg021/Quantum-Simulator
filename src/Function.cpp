#include "Function.hpp"
#include <cassert>

// TODO: probably also check if it's actually a Bijection
Bijection::Bijection(const std::vector<int>& _f): f(_f) {}

int Bijection::size() const {
    return f.size();
}

int Bijection::apply(int x) const {
    assert(x >= 0 && x < this->size());
    return f[x];
}

Bijection Bijection::controlled() const {
    int n = this->size();
    std::vector<int> g(2*n);
    for(int i = 0; i < n; i++){
        g[i] = i;
    }
    for(int i = n; i < 2*n; i++){
        g[i] = this->f[i-n] + n;
    }
    return Bijection(g);
}

// TODO: probably also check if it's actually a Rotation
Rotation::Rotation(const std::vector<std::complex<double>>& _f): f(_f) {}

int Rotation::size() const {
    return f.size();
}

std::complex<double> Rotation::getRotation(int x) const {
    return f[x];
}

Rotation Rotation::controlled() const {
    int n = this->size();
    std::vector<std::complex<double>> g(2*n, 1);
    for(int i = n; i < 2*n; i++){
        g[i] = this->f[i-n];
    }
    return Rotation(g);
}
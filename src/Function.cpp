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

// TODO: probably also check if it's actually a Rotation
Rotation::Rotation(const std::vector<std::complex<double>>& _f): f(_f) {}

int Rotation::size() const {
    return f.size();
}

std::complex<double> Rotation::getRotation(int x) const {
    return f[x];
}
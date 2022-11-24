#include "Bijection.hpp"
#include <cassert>

// TODO: probably also check if it's actually a Bijection
Bijection::Bijection(const std::vector<int>& _f): f(_f) {}

int Bijection::size(){
    return f.size();
}

int Bijection::apply(int x){
    assert(x >= 0 && x < this->size());
    return f[x];
}
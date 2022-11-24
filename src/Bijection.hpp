#ifndef BIJECTION_HPP
#define BIJECTION_HPP

#include <vector>

class Bijection{
    const std::vector<int> f;

    Bijection(const std::vector<int>& f);
    int size();
    int apply(int x);
};

#endif
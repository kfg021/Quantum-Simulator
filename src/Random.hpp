#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

static std::random_device rd;
static std::mt19937 rng(rd());
static std::uniform_real_distribution<double>dist(0, 1);

static double generateRandomDouble(){
    return dist(rng);
}

#endif
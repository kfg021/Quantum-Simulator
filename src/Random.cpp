#include "Random.hpp"
#include <random>

double generateRandomDouble(){
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<double>dist(0, 1);
    return dist(rng);
}
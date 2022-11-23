#include "Random.hpp"
#include <random>

std::random_device rd;
std::mt19937 rng(rd());

double generateRandomDouble(){
    static std::uniform_real_distribution<double>dist(0, 1);
    return dist(rng);
}

int generateRandomInt(int start, int end){
    static std::uniform_int_distribution<int> dist(start, end);
    return dist(rng);
}
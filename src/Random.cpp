#include "Random.hpp"
#include <random>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<double>dist(0, 1);

double generateRandomDouble(){
    return dist(rng);
}

int generateRandomInt(int start, int end){
    double rand = generateRandomDouble();
    return (int)(rand * (end - start + 1)) + start;
}
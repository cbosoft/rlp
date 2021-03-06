#pragma once

#include <iterator>
#include <random>

void seed(int s);
void seed(std::seed_seq s);
double urand(double min, double max);
double nrand(double mean, double std);
int urand(int min, int max);

// https://stackoverflow.com/a/16421677/10042036
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

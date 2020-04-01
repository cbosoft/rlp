#pragma once
#include <random>

void seed(std::seed_seq s);
double urand(double min, double max);
double nrand(double mean, double std);

#pragma once
#include <random>

void seed(int s);
void seed(std::seed_seq s);
double urand(double min, double max);
double nrand(double mean, double std);

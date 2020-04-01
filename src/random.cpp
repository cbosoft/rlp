#include "random.hpp"

static std::mt19937 e;

void seed(std::seed_seq s)
{
  e = std::mt19937(s);
}

double urand(double min, double max)
{
  std::uniform_real_distribution urd(min, max);
  return urd(e);
}

double nrand(double mean, double std)
{
  std::normal_distribution nd(mean, std);
  return nd(e);
}

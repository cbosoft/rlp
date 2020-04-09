#include "sieve.hpp"


UniformSieve::UniformSieve(double min)
  : min(min)
{
  // do nothing
}


double UniformSieve::get_size() const
{
  return urand(this->min, 1.0);
}


double UniformSieve::get_mean() const
{
  return 0.5*(1.0 + this->min);
}

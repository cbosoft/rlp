#include "sieve.hpp"


NormalSieve::NormalSieve(double std)
  : std(std)
{
  this->mean = 1.0 - (3.0*std);
}


double NormalSieve::get_size() const
{
  return nrand(this->mean, this->std);
}


double NormalSieve::get_mean() const
{
  return this->mean;
}

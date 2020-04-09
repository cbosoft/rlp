#include "sieve.hpp"


AlternatingBiSieve::AlternatingBiSieve(double ratio) 
{
  if (ratio > 1.0)
    ratio = 1.0/ratio;
  this->ratio = ratio;
}


double AlternatingBiSieve::get_size() const
{
  static bool prev = false;

  prev = !prev;

  if (prev) {
    return 1.0;
  }

  return this->ratio;
}


double AlternatingBiSieve::get_mean() const
{
  return 0.5*(1.0 + this->ratio);
}

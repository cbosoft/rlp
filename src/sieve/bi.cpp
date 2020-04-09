#include "sieve.hpp"


BiSieve::BiSieve(double ratio, double probability) 
{
  if (ratio > 1.0)
    ratio = 1.0/ratio;

  if (probability > 1.0)
    throw ArgumentError("Probability cannot exceed 1.0.");

  this->ratio = ratio;
  this->probability = probability;

}


double BiSieve::get_size() const
{
  double p = urand(0.0, 1.0);
  if (p < this->probability)
    return 1.0;
  return this->ratio;
}


double BiSieve::get_mean() const
{
  return this->probability + (1.0 - this->probability)*this->ratio;
}

#include "random.hpp"
#include "vec.hpp"

Vec3 vec_urand(double min, double max)
{
  return Vec3({urand(min, max), urand(min, max), urand(min, max)});
}


Vec3 vec_nrand(double mean, double std)
{
  return Vec3({nrand(mean, std), nrand(mean, std), nrand(mean, std)});
}

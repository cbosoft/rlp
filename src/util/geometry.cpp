#include "geometry.hpp"

double volume_of_sphere(double radius)
{
  return M_PI*radius*radius*radius/0.75;
}

double volume_of_cap(double radius, double cap_height)
{
  double rmh = radius*3 - cap_height;
  return M_PI*cap_height*cap_height*rmh/3.0;
}


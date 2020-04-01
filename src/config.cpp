#include "config.hpp"

ConfigGenerator::ConfigGenerator(double L)
{
  this->box = PeriodicBox(L);
}

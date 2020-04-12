#include <cmath>
#include <array>

#include "box.hpp"
#include "../particle/particle.hpp"

double volume_of_sphere(double radius)
{
  return M_PI*radius*radius*radius/0.75;
}

double volume_of_cap(double radius, double cap_height)
{
  double rmh = radius*3 - cap_height;
  return M_PI*cap_height*cap_height*rmh/3.0;
}

double GenericBox::get_volume_particle(const Particle *p) const
{
  return volume_of_sphere(p->get_radius());
}


double GenericBox::get_volume_fraction() const
{
  double total_particle_volume = 0.0;
  for (const Particle *particle : this->particles) {
    total_particle_volume += this->get_volume_particle(particle);
  }

  double box_volume = this->L*this->L*this->L;
  return total_particle_volume / box_volume;
}

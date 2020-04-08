#include "box.hpp"

double volume_of_sphere(double radius)
{
  return M_PI*0.75*radius*radius*radius;
}

double volume_of_cap(double radius, double cap_height)
{
  double rmh = radius*3 - cap_height;
  return M_PI*cap_height*cap_height*rmh/3.0;
}

double PeriodicBox::get_volume_particle(const Particle *p, double bottom, double top) const
{
  std::array<double, 3> position = p->get_position().as_array();
  double radius = p->get_radius();
  double diameter = radius+radius;

  std::vector<double> intersection_data;
  double pi = position[2];
  double min = bottom - (pi-radius);
  double max = (pi-radius) - top;
  if ((min > 0.0) and (min < diameter)) {
    intersection_data.push_back(min);
  }
  else if ((max > 0.0) && (max < diameter)) {
    intersection_data.push_back(max);
  }

  double volume = 0.0;
  switch (intersection_data.size()) {

    case 0:
      volume = volume_of_sphere(radius);
      break;

    case 1:
      volume = volume_of_cap(radius, intersection_data[0]);
      break;

    case 2:
      throw MathError("A particle is caught in two planes! Cannot calculate volume.");

  }

  return volume;
}


double PeriodicBox::get_volume_fraction() const
{
  double total_particle_volume = 0.0;
  double lowest_surface_height = this->get_lowest_surface_height();
  for (const Particle *particle : this->particles) {
    total_particle_volume += this->get_volume_particle(particle, 0.0, lowest_surface_height);
  }

  double box_volume = this->L*this->L*lowest_surface_height;
  return total_particle_volume / box_volume;
}

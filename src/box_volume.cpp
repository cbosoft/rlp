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

double volume_of_sphere_in_two_planes(double radius, double cap_height_1, double cap_height_2)
{
  // TODO: improve this approximation.
  (void) cap_height_2;
  double rmh = radius*3 - cap_height_1;
  return M_PI*cap_height_1*cap_height_1*rmh/3.0;
}

double volume_of_sphere_in_three_planes(double radius, double cap_height_1, double cap_height_2, double cap_height_3)
{
  // TODO: improve this approximation.
  (void) cap_height_2;
  (void) cap_height_3;
  double rmh = radius*3 - cap_height_1;
  return M_PI*cap_height_1*cap_height_1*rmh/3.0;
}








double PeriodicBox::get_volume_particle(const Particle *p, std::array<std::array<double, 2>, 3> limits) const
{
  std::array<double, 3> position = p->get_position().as_array();
  double radius = p->get_radius();

  std::vector<double> intersection_data;
  for (size_t i = 0; i < position.size(); i++) {
    double pi = position[i];
    double min = limits[i][0] - (pi-radius);
    double max = (pi-radius) - limits[i][1];
    if (min > 0.0) {
      intersection_data.push_back(min);
    }
    else if (max > 0.0) {
      intersection_data.push_back(max);
    }
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
      volume = volume_of_sphere_in_two_planes(radius, intersection_data[0], intersection_data[1]);
      break;

    case 3:
      volume = volume_of_sphere_in_three_planes(radius, intersection_data[0], intersection_data[1], intersection_data[2]);
      break;

  }

  return volume;
}


double PeriodicBox::get_volume_fraction() const
{
  double max_height = this->particles[0]->get_position().Z();
  for (const Particle *particle : this->particles) {
    double height = particle->get_position().Z();
    if (height > max_height)
      max_height = height;
  }

  double total_particle_volume = 0.0;
  for (const Particle *particle : this->particles) {
    total_particle_volume += this->get_volume_particle(particle, std::array<std::array<double, 2>, 3>({
          std::array<double, 2>({0.0, this->L}), 
          std::array<double, 2>({0.0, this->L}), 
          std::array<double, 2>({0.0, max_height})}));
  }

  double box_volume = this->L*this->L*max_height;
  return total_particle_volume / box_volume;
}

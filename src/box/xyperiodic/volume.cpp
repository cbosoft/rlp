#include <cmath>
#include <array>

#include "xyperiodicbox.hpp"
#include "../../particle/particle.hpp"
#include "../../util/geometry.hpp"


double XYPeriodicBox::get_volume_particle(const Particle *p) const
{
  std::array<double, 3> position = p->get_position().as_array();
  double radius = p->get_radius();

  std::vector<double> intersection_data;
  double pz = position[2];

  double bottom=0.0, top=this->L;
  bool outside = ((pz+radius < bottom) or (pz-radius > top));
  if (outside)
    return 0.0;
  
  bool intersects_bottom = pz-radius < bottom;
  bool intersects_top = pz+radius > top;
  if (intersects_top) {
    intersection_data.push_back(top - (pz - radius));
  }
  else if (intersects_bottom) {
    intersection_data.push_back(pz + radius - bottom);
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

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





double PeriodicBox::get_lowest_surface_height(int n) const
{
  (void)n;

  return this->L;

  // averaging method: disabled 2020-04-09
  // double tot = 0.0;
  // for (auto particle : this->particles) {
  //   tot += particle->get_position().Z();
  // }
  // double v = tot/double(this->particles.size());
  // return v;

  // meshing method: disabled 2020-04-08
  // double dL = this->L / double(n);
  // double **mesh = new double*[n];
  // for (int i = 0; i < n; i++)
  //   mesh[i] = new double[n];

  // for (auto particle : this->particles) {
  //   Vec3 v = particle->get_position();
  //   double px = v.X();
  //   double py = v.Y();
  //   double pz = v.Z();

  //   int ix = int(px/dL);
  //   int iy = int(py/dL);

  //   if ((ix > (n-1)) or (iy > (n-1)))
  //     throw MathError("Index error in get_lowest_surface_height");

  //   if (pz > mesh[ix][iy])
  //     mesh[ix][iy] = pz;
  // }

  // double lowest = mesh[0][0];
  // for (int i = 0; i < n; i++) {
  //   for (int j = 0; j < n; j++) {
  //     if (mesh[i][j] < lowest)
  //       lowest = mesh[i][j];
  //   }
  // }

  // for (int i = 0; i < n; i++)
  //   delete mesh[i];
  // delete mesh;

  // return lowest;
}

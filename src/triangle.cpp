#include "triangle.hpp"


Triangle::Triangle(Particle *a, Particle *b, Particle *c, PeriodicBox *box)
{
  this->particles[0] = a;
  this->particles[1] = b;
  this->particles[2] = c;
  this->box = box;

  // TODO precompute distances
}

bool Triangle::check_interacts_with(const Particle *p)
{
  int close_enough = 0;
  Vec2 ppos = p->get_position().restrict<2>();
  for (size_t i = 0; i < this->particles.size(); i++) {
    Particle *pi = this->particles[i];
    int j = i+1 >= this->particles.size() ? 0 : i+1;
    Particle *pj = this->particles[j];

    Vec2 ipos = pi->get_position().restrict<2>();
    Vec2 jpos = pj->get_position().restrict<2>();

    double d = this->box->get_effective_separation(ppos, ipos).magnitude();
    double d_comp = this->box->get_effective_separation(ipos, jpos).magnitude();
    if (d < this->particles[i]->get_radius() + p->get_radius())
      close_enough ++;

    if (d > d_comp)
      return false;

  }

  if (!close_enough) {
    // particle doesn't touch any of the corners
    return false;
  }

  return true;
}

Vec3 Triangle::get_interaction_result(const Particle *p)
{
}

double Triangle::get_sort_distance(const Particle *p)
{
  double totdist = 0.0;
  for (size_t i = 0; i < this->particles.size(); i++) {
    // no PBC (abs distance only)
    totdist += (p->get_position() - this->particles[i]->get_position()).magnitude();
  }
  return totdist / double(this->particles.size());
}


bool Triangle::trilaterate(double radius, Vec3 &rv)
{
  // https://stackoverflow.com/questions/1406375/finding-intersection-points-between-3-spheres/18654302#18654302
  Vec3 r_ji = this->particles[1]->get_position() - this->particles[0]->get_position();
  double d = r_ji.magnitude();
  Vec3 e_x = r_ji / d;
  Vec3 r_ki = this->particles[2]->get_position() - this->particles[0]->get_position();
  double i = e_x.dot(r_ki);
  Vec3 e_y = (r_ki - e_x*i).unit();
  Vec3 e_z = e_y.cross(e_x);
  double j = e_y.dot(r_ki);
  double r1 = this->particles[0]->get_radius()+radius, r2 = this->particles[1]->get_radius()+radius, r3 = this->particles[2]->get_radius()+radius;
  double x = (r1*r1 - r2*r2 + d*d) / (2*d);
  double y = (r1*r1 - r3*r3 -2*i*x + i*i + j*j) / (2*j);
  double z2 = r1*r1 - x*x - y*y;
  if (z2 < 0.0)
    return false;

  double z = std::pow(z2, 0.5);
  Vec3 solution_a = this->particles[0]->get_position() + e_x*x + e_y*y + e_z*z;
  Vec3 solution_b = this->particles[0]->get_position() + e_x*x + e_y*y - e_z*z;

  if (solution_a.get(2) > solution_b.get(2)) {
    rv = solution_a;
    return true;
  }
  rv = solution_b;
  return true;
}

#include "triangle.hpp"


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

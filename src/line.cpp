#include "line.hpp"


Line::Line(Particle *a, Particle *b, PeriodicBox *box)
{
  this->particles[0] = a;
  this->particles[1] = b;
  this->box = box;
  double dist = this->box->get_effective_separation(a->get_position(), b->get_position()).magnitude();
  this->supportable_size = dist - a->get_radius() - b->get_radius();
}


bool Line::check_interacts_with(const Particle *p)
{
  // could the line support the particle?
  if (this->supportable_size <= p->get_diameter())
    return false;

  // is the particle close enough to either line point?
  int close_enough = 0;
  Vec2 ppos = p->get_position().restrict<2>();
  for (size_t i = 0; i < this->particles.size(); i++) {
    Vec2 vpos = this->particles[i]->get_position().restrict<2>();
    double d = this->box->get_effective_separation(ppos, vpos).magnitude();
    if (d < this->particles[i]->get_radius() + p->get_radius()) {
      close_enough ++;
    }
  }

  if (!close_enough)
    return false;
  else if (close_enough == 2) {
    return true;
  }

  // I think just these two checks are sufficient. If the particle is not
  // interacting with both vertices of the line at once, then it will interact
  // with a single vertex and get pushed into the line (or not)
  return false;
}


Vec3 Line::get_interaction_result(const Particle *p)
{
  Vec2 S0 = p->get_position().restrict<2>();
  Vec2 R1 = this->particles[0]->get_position().restrict<2>();
  Vec2 R2 = this->particles[1]->get_position().restrict<2>();
  Vec2 dS = S0 - R1;
  Vec2 dR = R2 - R1;
  double dRmag = dR.magnitude();
  Vec2 dRu = dR / dRmag;
  Vec2 TS = dS.component_along(dR);
  Vec2 NS = S0 - TS; // or plus?
  Vec2 NSu = NS.unit();
  double a = dRmag;
  double b = this->particles[0]->get_radius() + p->get_radius();
  double c = this->particles[1]->get_radius() + p->get_radius();
  double costheta = (a*a + b*b - c*c) / (2*a*b);
  double theta = std::acos(costheta);
  double t = b*costheta;
  Vec2 C = R1 + (dRu*t);

  double l = b*std::sin(theta);
  Vec2 S = C + (NSu*l);

  return S.promote<3>(10.0);
}


double Line::get_sort_distance(const Particle *p)
{
  double totdist = 0.0;
  for (size_t i = 0; i < this->particles.size(); i++) {
    // no PBC (abs distance only)
    totdist += (p->get_position() - this->particles[i]->get_position()).magnitude();
  }
  return totdist / double(this->particles.size());
}

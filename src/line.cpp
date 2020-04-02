#include "line.hpp"


Line::Line(Particle *a, Particle *b, PeriodicBox *box)
{
  this->particles[0] = a;
  this->particles[1] = b;
  this->box = box;
  this->dist = this->box->get_effective_separation(a->get_position(), b->get_position()).magnitude();
  this->supportable_size = this->dist - a->get_radius() - b->get_radius();
}


bool Line::check_interacts_with(const Particle *p)
{
  // could the line support the particle?
  if (this->supportable_size > p->get_diameter()) {
    //std::cerr << "no because wide" << std::endl;
    return false;
  }

  // is the particle close enough to either line point?
  int close_enough = 0;
  Vec3 ppos3 = p->get_position();
  Vec2 ppos = ppos3.restrict<2>();
  for (size_t i = 0; i < this->particles.size(); i++) {
    Vec2 vpos = this->particles[i]->get_position().restrict<2>();
    double d = this->box->get_effective_separation(ppos, vpos).magnitude();

    // check if particle hits line not just end
    if (d > this->dist) {
      //std::cerr << "no because glancing" << std::endl;
      return false;
    }

    // check particle hits at all
    if (d < this->particles[i]->get_radius() + p->get_radius())
      close_enough ++;

    if (ppos3.get(2) <= this->particles[i]->get_position().get(2))
      return false;

  }

  if (!close_enough) {
    //std::cerr << "no because doesn't touch" << std::endl;
    return false;
  }

  return true;
}


Vec3 Line::get_interaction_result(const Particle *p)
{
  Vec2 S0 = p->get_position().restrict<2>();
  Vec2 R1 = this->particles[0]->get_position().restrict<2>();
  Vec2 R2 = this->particles[1]->get_position().restrict<2>();
  Vec2 dS = this->box->get_effective_separation(S0, R1);
  Vec2 dR = this->box->get_effective_separation(R2, R1);
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

  double h1 = this->particles[0]->get_position().get(2);
  double h2 = this->particles[2]->get_position().get(2);

  return S.promote<3>( (h1>h2)?h2:h1);
}


double Line::get_sort_distance(const Particle *p)
{
  double mindist = (p->get_position() - this->particles[0]->get_position()).magnitude();
  for (size_t i = 1; i < this->particles.size(); i++) {
    // no PBC (abs distance only)
    double dist = (p->get_position() - this->particles[i]->get_position()).magnitude();
    if (dist < mindist)
      mindist = dist;
  }
  return mindist*100.0 + 1;
}

#include "vertex.hpp"


Vertex::Vertex(Particle *p, PeriodicBox *box)
{
  this->particle = p;
  this->box = box;
}


bool Vertex::check_interacts_with(const Particle *p)
{
  Vec3 dr = this->box->get_effective_separation(this->particle->get_position(), p->get_position());
  return (dr.magnitude() < (this->particle->get_radius() + p->get_radius()));
}


Vec3 Vertex::get_interaction_result(const Particle *p)
{
  Vec2 dr_unit = (this->box->get_effective_separation(this->particle->get_position().restrict<2>(), p->get_position().restrict<2>())).unit();
  return (dr_unit * (this->particle->get_radius() + p->get_radius())).promote<3>(this->particle->get_position().get(2)) ;
}


double Vertex::get_sort_distance(const Particle *p)
{
  // don't take PBCinto account: need abs distance
  double dp = (p->get_position() - this->particle->get_position()).magnitude();
  return dp;
}

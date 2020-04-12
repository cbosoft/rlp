#include "vertex.hpp"


Vertex::Vertex(Particle *p, XYPeriodicBox *box)
{
  this->particle = p;
  this->box = box;
}


bool Vertex::check_interacts_with(const Particle *p)
{

  // box not periodic in z; don't need to get effective position to compare
  // heights
  if (p->get_position().get(2) <= this->particle->get_position().get(2))
    return false;

  Vec2 dr_xy = this->box->get_effective_separation(this->particle->get_position().restrict<2>(), p->get_position().restrict<2>());
  return (dr_xy.magnitude() < (this->particle->get_radius() + p->get_radius()));
}


Vec3 Vertex::get_interaction_result(const Particle *p)
{
  Vec2 S = p->get_position().restrict<2>();
start:
  Vec2 dr = (S - this->particle->get_position().restrict<2>());
  double drmag = dr.magnitude();

  if (drmag == 0.0) {
    S = S + vec2_urand(-1e-10, 1e-10);
    goto start;
  }

  Vec2 dr_unit = dr / drmag;
  Vec3 result_dr = (dr_unit * (this->particle->get_radius() + p->get_radius())).promote<3>(this->particle->get_position().get(2));
  Vec3 rv = result_dr + p->get_position();
  return this->box->get_effective_position(rv);
}


Vec3 Vertex::get_frictional_interaction_result(const Particle *p)
{
  double time = 0.0;
  Vec3 result;
  
  if (not this->get_two_particle_frictional_interaction(p, this->particle, time, result)) {
    throw IntersectionError("Intersection failure (vertex)");
  }

  return result;
}






double Vertex::get_sort_distance(const Particle *p)
{
  double dp = this->box->get_effective_separation(p->get_position(), this->particle->get_position()).magnitude();
  return dp * 101.0 + 3.0;
}


double Vertex::get_max_distance(const Particle *p)
{
  double dp = this->box->get_effective_separation(p->get_position(), this->particle->get_position()).magnitude();
  return dp;
}


double Vertex::get_min_distance(const Particle *p)
{
  double dp = this->box->get_effective_separation(p->get_position(), this->particle->get_position()).magnitude();
  return dp;
}


double Vertex::get_z_position()
{
  // box not periodic in z; don't need to get effective position to compare
  // heights
  return this->particle->get_position().get(2);
}


std::vector<Vec3> Vertex::get_extents()
{
  std::vector<Vec3> rv;
  rv.push_back(this->particle->get_position());
  return rv;
}

std::string Vertex::repr()
{
  std::stringstream ss;
  ss << this->get_type() << "(" << this->particle->get_position() << ")";
  return ss.str();
}

Vec3 Vertex::get_centre() const
{
  return this->particle->get_position();
}

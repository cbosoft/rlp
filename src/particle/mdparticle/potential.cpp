#include "mdparticle.hpp"

void MDParticle::interact(MDParticle *other)
{
  Vec3 force = Vec3({0.0, 0.0, 0.0});
  Vec3 dr = this->get_position() - other->get_position();
  double separation_limit = this->get_radius() + other->get_radius();

  // lennard jones but epsilon is 0.25
  
}

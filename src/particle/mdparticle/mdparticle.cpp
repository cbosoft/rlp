#include "mdparticle.hpp"

MDParticle::MDParticle(double diameter, Vec3 position, Vec3 velocity, Vec3 force)
  : Particle(diameter, position)
{
  this->velocity = velocity;
  this->force = force;
}

#pragma once
#include "../particle.hpp"

class MDParticle : virtual public Particle {

  Vec3 velocity;
  Vec3 force;

  public:

    MDParticle(double diameter, Vec3 position, Vec3 velocity, Vec3 force);
    ~MDParticle();

};

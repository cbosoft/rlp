#include "box.hpp"
#include "../particle/particle.hpp"


bool GenericBox::is_particle_frictional(const Particle *p) const
{
  return p->get_diameter() > this->friction_thresh;
}

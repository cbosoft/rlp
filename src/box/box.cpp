#include "box.hpp"
#include "../particle/particle.hpp"

// Constructors, destructors, setters and getters for XYPeriodicBox


GenericBox::GenericBox(double L, int verbosity, double friction_thresh)
{
  this->set_L(L);
  this->logger = Logger(verbosity);
  this->verbosity = verbosity;

  this->friction_thresh = friction_thresh;
}


GenericBox::~GenericBox()
{
  this->clear_particles();
}

void GenericBox::clear_particles()
{
  for (Particle *p : this->particles) {
    delete p;
  }
}





void GenericBox::set_L(double L)
{
  this->L = L;
  this->hL = L*0.5;
  this->invL = 1.0/L;
}
double GenericBox::get_L()
{
  return this->L;
}

const std::vector<Particle *> GenericBox::get_particles() const
{
  return this->particles;
}

int GenericBox::get_number_particles() const
{
  return this->particles.size();
}

void GenericBox::reserve(int n)
{
  this->particles.reserve(n);
}


double GenericBox::get_friction_fraction() const
{
  double c = 0.0;
  for (auto particle : this->particles) {
    if (is_particle_frictional(particle))
      c += 1.0;
  }
  return c / double(this->particles.size());
}

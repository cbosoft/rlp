#include "box.hpp"
#include "../particle/particle.hpp"

// Constructors, destructors, setters and getters for PeriodicBox


PeriodicBox::PeriodicBox(double L, int verbosity, const char *log_file_path, double friction_thresh)
{
  this->set_L(L);
  this->logger = Logger(log_file_path, verbosity);
  this->verbosity = verbosity;

  this->friction_thresh = friction_thresh;
  this->particles_are_seed = false;
}


PeriodicBox::~PeriodicBox()
{
  this->clear_particles();
  this->clear_arrangements();
}

void PeriodicBox::clear_particles()
{
  for (Particle *p : this->particles) {
    delete p;
  }
}


void PeriodicBox::clear_arrangements()
{
  for (ParticleArrangement *a : this->arrangements) {
    delete a;
  }
}

int PeriodicBox::get_number_arrangements() const
{
  return this->arrangements.size();
}


void PeriodicBox::set_L(double L)
{
  this->L = L;
  this->hL = L*0.5;
  this->invL = 1.0/L;
}
double PeriodicBox::get_L()
{
  return this->L;
}

const std::vector<Particle *> PeriodicBox::get_particles() const
{
  return this->particles;
}

std::list<ParticleArrangement *> PeriodicBox::get_arrangements() const
{
  return this->arrangements;
}

int PeriodicBox::get_number_particles() const
{
  return this->particles.size();
}

void PeriodicBox::reserve(int n)
{
  this->particles.reserve(n);
}

void PeriodicBox::set_particles_are_seed(bool value)
{
  this->particles_are_seed = value;
}

double PeriodicBox::get_friction_fraction() const
{
  double c = 0.0;
  for (auto particle : this->particles) {
    if (is_particle_frictional(particle))
      c += 1.0;
  }
  return c / double(this->particles.size());
}

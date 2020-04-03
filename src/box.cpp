#include "box.hpp"
#include "particle.hpp"

// Constructors, destructors, setters and getters for PeriodicBox


PeriodicBox::PeriodicBox(double L, int verbosity, const char *log_file_path)
{
  this->set_L(L);
  this->lowest_surface_height = 0.0;
  this->logger = Logger(log_file_path, verbosity);
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

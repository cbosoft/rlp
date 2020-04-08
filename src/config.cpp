#include <fstream>

#include "config.hpp"

ConfigGenerator::ConfigGenerator(double L, int verbosity, bool particles_are_seed, double friction_thresh)
{
  this->box = PeriodicBox(L, verbosity, nullptr, friction_thresh);
  this->verbosity = verbosity;
  this->box.set_particles_are_seed(particles_are_seed);
  this->sieve = new MonoSieve();
}


void ConfigGenerator::output_configuration(std::string file_path)
{
  const std::vector<Particle *> particles = this->box.get_particles();
  std::ofstream of(file_path, std::ios::trunc);

  of << this->box.get_L() << ", " << this->box.get_volume_fraction() << ", " << this->box.get_friction_fraction() << std::endl;
  for (const Particle *particle : particles) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << std::endl;
  }


}

void ConfigGenerator::set_sieve(Sieve *s)
{
  delete this->sieve;
  this->sieve = s;
}

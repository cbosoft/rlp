#include <fstream>

#include "config.hpp"
#include "exception.hpp"

ConfigGenerator::ConfigGenerator(double L, int verbosity, bool particles_are_seed, std::string output_file_path)
{
  this->box = PeriodicBox(L, verbosity);
  this->verbosity = verbosity;
  this->box.set_particles_are_seed(particles_are_seed);
  this->output_file_path = output_file_path;
  this->check_file_path();

  this->sieve = new MonoSieve();
}

void ConfigGenerator::check_file_path()
{
  std::ofstream of(this->output_file_path, std::ios::trunc);
  if (of.fail())
    throw IOError(Formatter() << "Error opening config file '" << this->output_file_path << "' for write! ", true);
}


void ConfigGenerator::output_configuration()
{
  std::ofstream of(this->output_file_path, std::ios::trunc);
  if (of.fail())
    throw IOError(Formatter() << "Error opening config file '" << this->output_file_path << "' for write! ", true);

  of << this->box.get_L() << ", " << this->box.get_volume_fraction() << std::endl;
  for (const Particle *particle : this->box.get_particles()) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << std::endl;
  }


}

void ConfigGenerator::set_sieve(Sieve *s)
{
  delete this->sieve;
  this->sieve = s;
}

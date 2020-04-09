#include <fstream>

#include "config.hpp"
#include "../util/exception.hpp"

ConfigGenerator::ConfigGenerator(double L, int verbosity, bool particles_are_seed, double friction_thresh, std::string output_file_path)
{
  this->box = PeriodicBox(L, verbosity, nullptr, friction_thresh);
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


void ConfigGenerator::output_configuration(bool failed)
{
  std::ofstream of(this->output_file_path, std::ios::trunc);
  if (of.fail())
    throw IOError(Formatter() << "Error opening config file '" << this->output_file_path << "' for write! ", true);

  std::string exit_status = (failed ? "error" : "exited_normally");
  of << this->box.get_L() << ", " << this->box.get_volume_fraction() << ", " << this->box.get_friction_fraction() << ", " << exit_status << std::endl;
  for (const Particle *particle : this->box.get_particles()) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << std::endl;
  }


}

void ConfigGenerator::set_sieve(Sieve *s)
{
  delete this->sieve;
  this->sieve = s;
}

const Sieve *ConfigGenerator::get_sieve() const
{
  return this->sieve;
}

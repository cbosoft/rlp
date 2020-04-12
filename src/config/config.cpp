#include <fstream>

#include "config.hpp"
#include "../util/exception.hpp"


ConfigGenerator::ConfigGenerator(int verbosity, std::string output_file_path)
{
  this->verbosity = verbosity;
  this->output_file_path = output_file_path;
  this->check_file_path();

  this->box = nullptr;
  this->sieve = nullptr;
}

ConfigGenerator::~ConfigGenerator()
{

  if (this->box != nullptr)
    delete this->box;

  if (this->sieve != nullptr)
    delete this->sieve;

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
  of << this->box->get_L() << ", " << this->box->get_volume_fraction() << ", " << this->box->get_friction_fraction() << ", " << exit_status << std::endl;
  for (const Particle *particle : this->box->get_particles()) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << ", " << particle->get_contact_number() << std::endl;
  }

}


void ConfigGenerator::set_sieve(Sieve *s)
{
  if (this->sieve != nullptr)
    delete this->sieve;

  this->sieve = s;
}


void ConfigGenerator::set_box(GenericBox *b)
{
  if (this->box != nullptr)
    delete this->box;

  this->box = b;
}


const Sieve *ConfigGenerator::get_sieve() const
{
  return this->sieve;
}

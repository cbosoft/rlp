#include <fstream>

#include "config.hpp"

ConfigGenerator::ConfigGenerator(double L, int verbosity)
{
  this->box = PeriodicBox(L, verbosity);
}


void ConfigGenerator::output_configuration(std::string file_path)
{
  const std::vector<Particle *> particles = this->box.get_particles();
  std::ofstream of(file_path, std::ios::trunc);

  of << this->box.get_L() << std::endl;
  for (const Particle *particle : particles) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << std::endl;
  }

}

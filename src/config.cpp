#include <fstream>

#include "config.hpp"

ConfigGenerator::ConfigGenerator(double L)
{
  this->box = PeriodicBox(L);
}


void ConfigGenerator::output_configuration(std::string file_path)
{
  const std::vector<Particle *> particles = this->box.get_particles();
  std::ofstream of(file_path, std::ios::trunc);

  for (const Particle *particle : particles) {
    of << particle->get_diameter() << ", " << particle->get_position().to_csv() << std::endl;
  }

}

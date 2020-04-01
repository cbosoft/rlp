#pragma once

#include "vec.hpp"
#include "particle.hpp"
#include "box.hpp"

class ConfigGenerator {

  private:

    PeriodicBox box;

  public:

    ConfigGenerator(double L);

    void generate_particles(int n);
    void generate_particle();
    void output_configuration(std::string filepath);

};

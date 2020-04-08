#pragma once

#include "vec.hpp"
#include "particle.hpp"
#include "box.hpp"

class ConfigGenerator {

  private:

    PeriodicBox box;
    int verbosity;

  public:

    ConfigGenerator(double L, int verbosity, bool particles_are_seed);

    void generate_particles(int n, int error_tolerance);
    void generate_particle();
    void output_configuration(std::string filepath);

};

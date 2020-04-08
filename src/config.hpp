#pragma once

#include "vec.hpp"
#include "particle.hpp"
#include "box.hpp"
#include "sieve.hpp"

class ConfigGenerator {

  private:

    PeriodicBox box;
    Sieve *sieve;
    int verbosity;

  public:

    ConfigGenerator(double L, int verbosity, bool particles_are_seed);

    void generate_particles(int n, int error_tolerance);
    void generate_particle();
    void output_configuration(std::string filepath);
    void set_sieve(Sieve *s);

};

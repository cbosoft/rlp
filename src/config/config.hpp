#pragma once

#include "../util/vec.hpp"
#include "../particle/particle.hpp"
#include "../box/box.hpp"
#include "../sieve/sieve.hpp"

class ConfigGenerator {

  private:

    GenericBox *box;
    Sieve *sieve;
    int verbosity;
    std::string output_file_path;

    void check_file_path();

  public:

    ConfigGenerator(int verbosity, std::string output_file_path);

    void generate_particles(int n, int error_tolerance);
    void generate_particle();
    void output_configuration(bool fail=false);
    void set_sieve(Sieve *s);
    void set_box(GenericBox *b);
    const Sieve *get_sieve() const;

};

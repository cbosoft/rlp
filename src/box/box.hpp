#pragma once

#include <vector>

#include "../util/logger.hpp"

class Particle;
class GenericBox {

  protected:

    std::vector<Particle *> particles;
    int verbosity;
    Logger logger;
    double friction_thresh;
    double L, hL, invL;

    void log(std::string s) { this->logger.log(s); }
    void log(std::string s, int minimum_verbosity) { this->logger.log(s, minimum_verbosity); }

    virtual double get_volume_particle(const Particle *p) const;
    bool is_particle_frictional(const Particle *p) const;
    void clear_particles();

  public:

    GenericBox(double L, int verbosity, double friction_thresh);
    virtual ~GenericBox();

    void reserve(int n);
    virtual void add_particle(Particle *p) =0;
    virtual void finish();

    void set_L(double L);

    double get_L();
    int get_number_particles() const;
    double get_friction_fraction() const;
    double get_volume_fraction() const;
    const std::vector<Particle *> get_particles() const;
};

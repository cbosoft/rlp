#pragma once

#include <vector>

#include "vec.hpp"
#include "arrangement.hpp"

class Particle;
class PeriodicBox {

  private:

    double L, hL, invL;
    std::vector<Particle *> particles;
    std::vector<ParticleArrangement *> arrangements;

    void clear_particles();
    void clear_arrangements();
    void settle_particle(Particle *p, int n=0, int recursion_limit=100);
    void update_arrangements();

  public:

    PeriodicBox() : PeriodicBox(0.0) {}
    PeriodicBox(double L);
    ~PeriodicBox();

    Vec3 get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept;
    Vec3 get_effective_position(const Vec3 &a) const noexcept;

    void set_L(double L);
    double get_L();
    void add_particle(Particle *p);

};

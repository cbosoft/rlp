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

    template<int N>
    VecN<N> get_effective_separation(const VecN<N> &a, const VecN<N> &b) const noexcept;
    Vec3 get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept;
    Vec2 get_effective_separation(const Vec2 &a, const Vec2 &b) const noexcept;
    template<int N>
    VecN<N> get_effective_position(const VecN<N> &a) const noexcept;
    Vec3 get_effective_position(const Vec3 &a) const noexcept;
    Vec2 get_effective_position(const Vec2 &a) const noexcept;

    void set_L(double L);
    double get_L();
    void add_particle(Particle *p);

};

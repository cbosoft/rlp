#pragma once

#include <array>
#include "arrangement.hpp"
#include "box.hpp"

class Triangle : public virtual ParticleArrangement {

  private:
    std::array<Particle *, 3> particles;
    PeriodicBox *box;

    bool trilaterate(double radius, Vec3 &v);

  public:
    Triangle(Particle *a, Particle *b, Particle *c, PeriodicBox *box);

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    bool is_final() override { return true; }
};

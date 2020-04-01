#pragma once

#include <array>
#include "arrangement.hpp"
#include "box.hpp"


class Line : public virtual ParticleArrangement {

  private:
    double supportable_size;
    std::array<Particle *, 2> particles;
    PeriodicBox *box;

  public:
    Line(Particle *a, Particle *b, PeriodicBox *box);

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    bool is_final() override { return false; }
};

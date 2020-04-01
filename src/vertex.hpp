#pragma once

#include "box.hpp"
#include "arrangement.hpp"

class Vertex : public virtual ParticleArrangement {

  private:
    Particle *particle;
    PeriodicBox *box;

  public:
    Vertex(Particle *p, PeriodicBox *box);
    ~Vertex() {}

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    bool is_final() override { return false; }
    
};

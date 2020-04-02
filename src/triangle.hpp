#pragma once

#include <array>
#include "arrangement.hpp"
#include "box.hpp"

class Triangle : public virtual ParticleArrangement {

  private:
    std::array<Particle *, 3> particles;
    std::array<double, 3> edge_distances; // 0-1, 1-2, 2-0
    PeriodicBox *box;

    bool trilaterate(double radius, Vec3 &v);

  public:
    Triangle(Particle *a, Particle *b, Particle *c, PeriodicBox *box);

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    double get_z_position();
    bool covers(ParticleArrangement* arr);
    std::vector<Vec3> get_extents();
    std::string repr() override;

    bool is_final() override { return true; }
    std::string get_type() override { return "Triangle"; }

    friend class TriangleTrilaterationTest;
};

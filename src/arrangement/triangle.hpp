#pragma once

#include <array>

#include "arrangement.hpp"
#include "../box/box.hpp"

class Triangle : public virtual ParticleArrangement {

  private:
    std::array<Particle *, 3> particles;
    std::array<double, 3> edge_distances; // 0-1, 1-2, 2-0
    std::array<double, 3> internal_angles; // 0-1, 1-2, 2-0
    std::array<Vec2, 3> edges; // 0-1, 1-2, 2-0
    PeriodicBox *box;

    bool trilaterate(double radius, Vec3 &v);
    int get_closest_particle_index(const Particle *p);

  public:
    Triangle(Particle *a, Particle *b, Particle *c, PeriodicBox *box);

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    Vec3 get_frictional_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    double get_max_distance(const Particle *p) override;
    double get_min_distance(const Particle *p) override;
    double get_z_position();
    bool covers(ParticleArrangement* arr);
    std::vector<Vec3> get_extents();
    std::string repr() override;
    Vec3 get_centre() const override;

    bool is_final() override { return true; }
    std::string get_type() override { return "Triangle"; }
    int get_complexity() override { return 3; }

    friend class TriangleTrilaterationTest;
};

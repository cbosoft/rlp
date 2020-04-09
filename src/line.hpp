#pragma once

#include <array>
#include "arrangement.hpp"
#include "box.hpp"


class Line : public virtual ParticleArrangement {

  private:
    double dist, supportable_size;
    std::array<Particle *, 2> particles;
    PeriodicBox *box;

  public:
    Line(Particle *a, Particle *b, PeriodicBox *box);

    bool check_interacts_with(const Particle *p) override;
    Vec3 get_interaction_result(const Particle *p) override;
    Vec3 get_frictional_interaction_result(const Particle *p) override;
    double get_sort_distance(const Particle *p) override;
    double get_max_distance(const Particle *p) override;
    double get_min_distance(const Particle *p) override;
    double get_z_position();
    bool covers(ParticleArrangement *arr) { (void)arr; return false; }
    std::vector<Vec3> get_extents();
    std::string repr() override;
    Vec3 get_centre() const override;

    bool is_final() override { return false; }
    std::string get_type() override { return "Line"; }
    int get_complexity() override { return 2; }
};

#pragma once

#include "arrangement.hpp"
#include "../box/xyperiodic/xyperiodicbox.hpp"

class Vertex : public virtual ParticleArrangement {

  private:
    Particle *particle;
    XYPeriodicBox *box;

  public:
    Vertex(Particle *p, XYPeriodicBox *box);
    ~Vertex() {}

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
    std::string get_type() override { return "Vertex"; }
    int get_complexity() override { return 1; }

};

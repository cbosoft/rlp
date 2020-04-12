#pragma once

#include <vector>
#include <list>

#include "../../arrangement/arrangement.hpp"
#include "../../util/vec.hpp"
#include "../box.hpp"

class XYPeriodicBox : public virtual GenericBox {

  private:

    std::list<ParticleArrangement *> arrangements;
    bool particles_are_seed;

    void check_particle_set_settled(Particle *p);
    void clear_arrangements();
    void settle_particle(Particle *p, int n=0, int recursion_limit=100);
    void update_arrangements();
    ParticleArrangement *sort_and_filter_arrangements(std::list<ParticleArrangement *> &arrangements, const Particle *particle) const;

    double get_volume_particle(const Particle *p) const override;

    template<int N>
    VecN<N> get_effective_separation(const VecN<N> &a, const VecN<N> &b) const noexcept;
    Vec3 get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept;
    Vec2 get_effective_separation(const Vec2 &a, const Vec2 &b) const noexcept;
    template<int N>
    VecN<N> get_effective_position(const VecN<N> &a) const noexcept;
    Vec3 get_effective_position(const Vec3 &a) const noexcept;
    Vec2 get_effective_position(const Vec2 &a) const noexcept;

  public:

    XYPeriodicBox() : XYPeriodicBox(0.0) {}
    XYPeriodicBox(double L, int verbosity=1, double friction_thresh=2.0) : GenericBox(L, verbosity, friction_thresh) {};
    ~XYPeriodicBox();

    void add_particle(Particle *p) override;

    int get_number_arrangements() const;
    std::list<ParticleArrangement *> get_arrangements() const;
    void add_particle_no_settle(Particle *p);
    bool stable_sites_remaining() const;

    void set_particles_are_seed(bool value=true);

    friend class Triangle;
    friend class Line;
    friend class Vertex;

    friend class VolumeBasicTest;
    friend class VolumeWallTest;
    friend class VolumeCapTest;
    friend class VertexInteractionTest;


};

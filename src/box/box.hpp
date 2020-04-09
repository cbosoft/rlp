#pragma once

#include <vector>
#include <list>

#include "../arrangement/arrangement.hpp"
#include "../util/vec.hpp"
#include "../util/logger.hpp"

class Particle;
class PeriodicBox {

  private:

    int verbosity;
    double L, hL, invL;
    std::vector<Particle *> particles;
    std::list<ParticleArrangement *> arrangements;
    Logger logger;
    bool particles_are_seed;
    double friction_thresh;

    void check_particle_set_settled(Particle *p);
    void clear_particles();
    void clear_arrangements();
    void settle_particle(Particle *p, int n=0, int recursion_limit=100);
    void update_arrangements();
    ParticleArrangement *sort_and_filter_arrangements(std::list<ParticleArrangement *> &arrangements, const Particle *particle) const;

    double get_volume_particle(const Particle *p, double bottom, double top) const;
    double get_lowest_surface_height(int n=4) const;
    bool is_particle_frictional(const Particle *p) const;

  public:

    PeriodicBox() : PeriodicBox(0.0) {}
    PeriodicBox(double L, int verbosity=1, const char *log_file_path=nullptr, double friction_thresh=2.0);
    ~PeriodicBox();

    void reserve(int n);

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
    int get_number_arrangements() const;
    std::list<ParticleArrangement *> get_arrangements() const;
    int get_number_particles() const;
    double get_volume_fraction() const;
    double get_friction_fraction() const;
    void add_particle_no_settle(Particle *p);
    bool stable_sites_remaining() const;

    const std::vector<Particle *> get_particles() const;

    void set_particles_are_seed(bool value=true);

    void log(std::string s) { this->logger.log(s); }
    void log(std::string s, int minimum_verbosity) { this->logger.log(s, minimum_verbosity); }

    friend class VolumeBasicTest;
    friend class VolumeWallTest;
    friend class VolumeCapTest;


};

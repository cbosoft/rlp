#pragma once

#include <vector>
#include <list>

#include "vec.hpp"
#include "arrangement.hpp"
#include "logger.hpp"

class Particle;
class PeriodicBox {

  private:

    double L, hL, invL;
    std::vector<Particle *> particles;
    std::list<ParticleArrangement *> arrangements;
    double lowest_surface_height;
    Logger logger;

    void clear_particles();
    void clear_arrangements();
    void settle_particle(Particle *p, int n=0, int recursion_limit=100);
    void update_arrangements();

  public:

    PeriodicBox() : PeriodicBox(0.0) {}
    PeriodicBox(double L, int verbosity=1, const char *log_file_path=nullptr);
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
    int get_number_arrangements() const;
    std::list<ParticleArrangement *> get_arrangements() const;
    double get_lowest_surface_height(double thresh=2.0);

    const std::vector<Particle *> get_particles() const;

    void log(std::string s) { this->logger.log(s); }
    void log(std::string s, int minimum_verbosity) { this->logger.log(s, minimum_verbosity); }


};

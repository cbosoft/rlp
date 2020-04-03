#pragma once

#include <vector>

#include "vec.hpp"

class Particle {

  private:

    Vec3 position;
    double diameter, radius;
    bool settled, frictional;

    void set_diameter(double d);

  public:
    std::vector<const Particle *> neighbours;

    Particle(double diameter, Vec3 position);
    std::string repr() const noexcept;

    void set_z(double z) noexcept;
    void set_position(Vec3 position);
    Vec3 get_position() const noexcept;


    double get_diameter() const noexcept;
    double get_radius() const noexcept;
    bool is_settled() const noexcept;
    void set_settled(bool v=true) noexcept;
    bool is_frictional() const noexcept;
    void set_frictional(bool v=true) noexcept;

    void add_neighbour(const Particle *) noexcept;
};

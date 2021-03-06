#pragma once

#include <vector>

#include "../util/vec.hpp"

class ParticleArrangement;
class Particle {

  private:

    Vec3 position;
    double diameter, radius;
    bool settled, frictional;
    ParticleArrangement *previous_interaction;

    void set_diameter(double d);

  public:
    std::vector<const Particle *> neighbours;

    Particle(double diameter, Vec3 position);
    std::string repr() const noexcept;

    void set_z(double z) noexcept;
    void set_position(Vec3 position);
    Vec3 get_position() const noexcept;


    int get_contact_number() const noexcept;
    double get_diameter() const noexcept;
    double get_radius() const noexcept;
    bool is_settled() const noexcept;
    void set_settled(bool v=true) noexcept;
    bool is_frictional() const noexcept;
    void set_frictional(bool v=true) noexcept;
    void set_previous_interacting(ParticleArrangement *arr) noexcept;
    ParticleArrangement *get_previous_interacting() const noexcept;

    void add_neighbour(const Particle *) noexcept;
};

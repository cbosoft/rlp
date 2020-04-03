#include "exception.hpp"
#include "particle.hpp"

// constructors, destructors, getters, setters

Particle::Particle(double diameter, Vec3 position)
{
  this->set_diameter(diameter);
  this->set_position(position);
  this->settled = false;
  this->frictional = false;
  this->previous_interaction = nullptr;
}

void Particle::set_diameter(double diameter)
{
  this->diameter = diameter;
  this->radius = diameter*0.5;
}

void Particle::set_z(double z) noexcept
{
  this->position.set(2, z);
}

void Particle::set_position(Vec3 position)
{
  if (position.has_nan()) {
    throw MathError(Formatter() << "Particle position was set to a non-feasible vector: " << position << ".");
  }

  this->position = position;
}

Vec3 Particle::get_position() const noexcept
{
  return this->position;
}

double Particle::get_diameter() const noexcept
{
  return this->diameter;
}

double Particle::get_radius() const noexcept
{
  return this->radius;
}

bool Particle::is_settled() const noexcept
{
  return this->settled;
}

void Particle::set_settled(bool v) noexcept
{
  this->settled = v;
}

bool Particle::is_frictional() const noexcept
{
  return this->frictional;
}

void Particle::set_frictional(bool v) noexcept
{
  this->frictional = v;
}

void Particle::add_neighbour(const Particle *p) noexcept
{
  this->neighbours.push_back(p);
}

std::string Particle::repr() const noexcept
{
  std::stringstream ss;
  ss << "Particle (" << this->diameter << "," << this->position << ")";
  return ss.str();
}

void Particle::set_previous_interacting(ParticleArrangement *arr) noexcept
{
  this->previous_interaction = arr;
}

ParticleArrangement *Particle::get_previous_interacting() const noexcept
{
  return this->previous_interaction;
}

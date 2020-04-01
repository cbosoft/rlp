#include "particle.hpp"

// constructors, destructors, getters, setters

Particle::Particle(double diameter, Vec3 position)
{
  this->set_diameter(diameter);
  this->set_position(position);
  this->settled = false;
  this->frictional = false;
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

void Particle::set_position(Vec3 position) noexcept
{
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

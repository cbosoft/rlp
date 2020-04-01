#include "box.hpp"

Vec3 PeriodicBox::get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept
{
  Vec3 dr = a - b;
  dr = dr - (dr*this->invL).nearbyint()*this->L;
  return dr;
}

Vec3 PeriodicBox::get_effective_position(const Vec3 &a) const noexcept
{
  Vec3 rv = a;
  rv -= (rv * this->invL).floor() * this->L;
  return rv;
}

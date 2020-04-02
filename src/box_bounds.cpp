#include "box.hpp"

template<>
VecN<2> PeriodicBox::get_effective_separation(const VecN<2> &a, const VecN<2> &b) const noexcept
{
  // only periodic in XY! not periodic in Z
  VecN<2> dr = a - b;
  dr = dr - (dr*this->invL).nearbyint()*this->L;
  return dr;
}

template<>
VecN<2> PeriodicBox::get_effective_position(const VecN<2> &a) const noexcept
{
  // only periodic in XY! not periodic in Z
  VecN<2> rv = a;
  rv -= (rv * this->invL).floor() * this->L;
  return rv;
}

template<>
Vec3 PeriodicBox::get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept
{
  // only periodic in XY! not periodic in Z
  VecN<3> dr_3d = a - b;
  Vec2 dr = dr_3d.restrict<2>();
  dr = dr - (dr*this->invL).nearbyint()*this->L;
  return dr.promote<3>(dr_3d.get(2));
}

template<>
Vec3 PeriodicBox::get_effective_position(const Vec3 &a) const noexcept
{
  // only periodic in XY! not periodic in Z
  Vec3 rv_3d = a;
  Vec2 rv = rv_3d.restrict<2>();
  rv -= (rv * this->invL).floor() * this->L;
  return rv.promote<3>(rv_3d.get(2));
}

Vec3 PeriodicBox::get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept { return this->get_effective_separation<>(a, b); }
Vec2 PeriodicBox::get_effective_separation(const Vec2 &a, const Vec2 &b) const noexcept { return this->get_effective_separation<>(a, b); }
Vec3 PeriodicBox::get_effective_position(const Vec3 &a) const noexcept { return this->get_effective_position<>(a); }
Vec2 PeriodicBox::get_effective_position(const Vec2 &a) const noexcept { return this->get_effective_position<>(a); }

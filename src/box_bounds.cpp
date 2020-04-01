#include "box.hpp"

template<int N>
VecN<N> PeriodicBox::get_effective_separation(const VecN<N> &a, const VecN<N> &b) const noexcept
{
  VecN<N> dr = a - b;
  dr = dr - (dr*this->invL).nearbyint()*this->L;
  return dr;
}

template<int N>
VecN<N> PeriodicBox::get_effective_position(const VecN<N> &a) const noexcept
{
  VecN<N> rv = a;
  rv -= (rv * this->invL).floor() * this->L;
  return rv;
}

Vec3 PeriodicBox::get_effective_separation(const Vec3 &a, const Vec3 &b) const noexcept { return this->get_effective_separation<>(a, b); }
Vec2 PeriodicBox::get_effective_separation(const Vec2 &a, const Vec2 &b) const noexcept { return this->get_effective_separation<>(a, b); }
Vec3 PeriodicBox::get_effective_position(const Vec3 &a) const noexcept { return this->get_effective_position<>(a); }
Vec2 PeriodicBox::get_effective_position(const Vec2 &a) const noexcept { return this->get_effective_position<>(a); }

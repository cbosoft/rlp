#pragma once

#include <string>
#include <sstream>
#include <cmath>
#include <array>

#include "exception.hpp"
#include "epsilon.hpp"

template<int N>
class VecN {

  private:

    std::array<double, N> v;

  public:

    VecN() {
      for (int i = 0; i < N; i++) {
        this->v[i] = 0.0;
      }
    }
    VecN(std::array<double, N> v) : v(v) {}

    std::string repr(bool ends=true, const char *separator=", ", const char *lend="[", const char *rend="]") const noexcept
    {
      std::stringstream ss;
      if (ends)
        ss << lend;
      for (int i = 0; i < N-1; i++) {
        ss << v[i] << separator;
      }
      ss << v[N-1];

      if (ends)
        ss << rend;
      return ss.str();
    }
    std::string to_tsv() { return repr(false, "\t"); }
    std::string to_ssv() { return repr(false, " "); }
    std::string to_csv() { return repr(false); }
    std::string to_yaml() { return repr(); }

    template<int I>
    friend std::ostream& operator <<(std::ostream &os, const VecN<I> v);


    VecN<N> add(const VecN<N> &v) const noexcept
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = this->v[i] + v.v[i];
      }
      return rv;
    }


    VecN<N> subtract(const VecN<N> &v) const noexcept
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = this->v[i] - v.v[i];
      }
      return rv;
    }


    VecN<N> cross(const VecN<N> &v) const
    {
      if (N != 3) {
        throw MathError("Cross product only implemented for 3D vectors.");
      }

      VecN<N> rv;
      rv.v[0] = (this->v[1]*v.v[2]) - (this->v[2]*v.v[1]);
      rv.v[1] = (this->v[2]*v.v[0]) - (this->v[0]*v.v[2]);
      rv.v[2] = (this->v[0]*v.v[1]) - (this->v[1]*v.v[0]);
      return rv;
    }

    double dot(const VecN<N> &v) const noexcept
    {
      double rv = 0.0;
      for (int i = 0; i < N; i++) {
        rv += this->v[i]*v.v[i];
      }
      return rv;
    }

    VecN<N> scalar_multiply(double d) const noexcept
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = this->v[i] * d;
      }
      return rv;
    }

    VecN<N> scalar_divide(double d) const noexcept
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = this->v[i] / d;
      }
      return rv;
    }

    double magnitude2() const noexcept
    {
      double tot = 0.0;
      for (int i = 0; i < N; i++) {
        tot += this->v[i] * this->v[i];
      }
      return tot;
    }

    double magnitude() const noexcept
    {
      return std::pow(this->magnitude2(), 0.5);
    }

    VecN<N> unit() const noexcept
    {
      VecN<N> rv(this->v);
      return rv / rv.magnitude();
    }

    VecN<N> component_along(const VecN<N> &other) const noexcept
    {
      VecN<N> unit_other = other.unit();
      return unit_other * other.dot(*this);
    }


    double angle_between(const VecN<N> &other) const noexcept
    {
      double costheta = this->unit().dot(other.unit());
      return std::acos(costheta);
    }

    VecN<N> floor() const noexcept
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = std::floor(this->v[i]);
      }
      return rv;
    }

    VecN<N> nearbyint() const
    {
      VecN<N> rv;
      for (int i = 0; i < N; i++) {
        rv.v[i] = std::nearbyint(this->v[i]);
      }
      return rv;
    }

    const std::array<double, N> &as_array() const
    {
      return this->v;
    }

    VecN<N> operator+(const VecN<N> &v) const { return this->add(v); }
    VecN<N> operator-(const VecN<N> &v) const { return this->subtract(v); }
    VecN<N> operator*(const VecN<N> &v) const { return this->cross(v); }
    VecN<N> operator*(double d) const { return this->scalar_multiply(d); }
    VecN<N> operator/(double d) const { return this->scalar_divide(d); }
    VecN<N> operator%(double divisor) const 
    {
      VecN<N> rv;
      for (int i = 0; i <N; i++) {
        rv.v[i] = std::fmod(this->v[0], divisor);
      }
      return rv;
    }
    void operator-=(const VecN<N> &v)
    {
      for (int i = 0; i < N; i++) {
        this->v[i] -= v.v[i];
      }
    }
    void operator-=(double d)
    {
      for (int i = 0; i < N; i++) {
        this->v[i] -= d;
      }
    }

    bool operator==(const VecN<N> &other) const
    {
      for (int i = 0; i < N; i++) {
        if (not FLOAT_EQ(this->v[i], other.v[i]))
          return false;
      }
      return true;
    }

    bool operator!=(const VecN<N> &other) const
    {
      return not ((*this) == other);
    }

    void set(int i, double v)
    {
      if (i >= N)
        throw IndexError(Formatter() << "Index " << i << " invalid for vector of size " << N << ".");

      this->v[i] = v;
    }

    template<int I>
    VecN<I> restrict()
    {
      assert( I < N, "restricted size must be less than or equal to vector size." );
      std::array<double, I> rv;
      for (int i = 0; i < I; i++) {
        rv[i] = this->v[i];
      }
      return VecN<I>(rv);
    }

    template<int I>
    VecN<I> promote(double fill)
    {
      assert( I > N, "restricted size must be less than or equal to vector size." );
      std::array<double, I> rv;
      for (int i = 0; i < N; i++) {
        rv[i] = this->v[i];
      }
      for (int i = N; i < I; i++) {
        rv[i] = fill;
      }
      return VecN<I>(rv);
    }
};

typedef VecN<3> Vec3;
typedef VecN<2> Vec2;

Vec3 vec_urand(double min, double max);
Vec3 vec_nrand(double mean, double std);

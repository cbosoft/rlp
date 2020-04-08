#pragma once
#include "random.hpp"

class Sieve {

  public:

    Sieve () {}
    virtual ~Sieve() { };

    virtual double get_size() const =0;
};


class MonoSieve : virtual public Sieve {

  private:

    double diameter;

  public:

    MonoSieve(double diameter=1.0) 
      : diameter(diameter) 
    {
      //
    }

    double get_size() const
    {
      return this->diameter;
    }

};



class BiSieve : virtual public Sieve {

  private:

    double diameter1, diameter2;

  public:

    BiSieve(double diameter1=1.0, double diameter2=0.5) 
      : diameter1(diameter1), diameter2(diameter2) 
    {
      //
    }

    double get_size() const
    {
      if (urand(0,1)) {
        return this->diameter1;
      }

      return this->diameter2;
    }
};



class AlternatingBiSieve : virtual public Sieve {

  private:

    double diameter1, diameter2;

  public:

    AlternatingBiSieve(double diameter1=1.0, double diameter2=0.5) 
      : diameter1(diameter1), diameter2(diameter2)
    {
      //
    }

    double get_size() const
    {
      static bool prev = false;

      prev = !prev;

      if (prev) {
        return this->diameter1;
      }

      return this->diameter2;
    }
};

#pragma once

#include "../util/random.hpp"
#include "../util/exception.hpp"

class Sieve {

  public:

    Sieve () {}
    virtual ~Sieve() { };

    virtual double get_size() const =0;
    virtual double get_mean() const =0;
};


class MonoSieve : virtual public Sieve {

  public:

    MonoSieve() 
    {
      //
    }

    double get_size() const
    {
      return 1.0;
    }

    double get_mean() const
    {
      return 1.0;
    }

};



class BiSieve : virtual public Sieve {

  private:

    double ratio, probability;

  public:

    BiSieve(double ratio, double probability) 
    {
      if (ratio > 1.0)
        ratio = 1.0/ratio;

      if (probability > 1.0)
        throw ArgumentError("Probability cannot exceed 1.0.");

      this->ratio = ratio;
      this->probability = probability;

    }

    double get_size() const
    {
      double p = urand(0.0, 1.0);
      if (p < this->probability)
        return 1.0;
      return this->ratio;
    }

    double get_mean() const
    {
      return this->probability + (1.0 - this->probability)*this->ratio;
    }
};



class AlternatingBiSieve : virtual public Sieve {

  private:

    double ratio;

  public:

    AlternatingBiSieve(double ratio) 
    {
      if (ratio > 1.0)
        ratio = 1.0/ratio;
      this->ratio = ratio;
    }

    double get_size() const
    {
      static bool prev = false;

      prev = !prev;

      if (prev) {
        return 1.0;
      }

      return this->ratio;
    }

    double get_mean() const
    {
      return 0.5*(1.0 + this->ratio);
    }
};

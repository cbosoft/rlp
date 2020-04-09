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

    MonoSieve();
    double get_size() const;
    double get_mean() const;

};



class BiSieve : virtual public Sieve {

  private:

    double ratio, probability;

  public:

    BiSieve(double ratio, double probability);
    double get_size() const;
    double get_mean() const;

};



class AlternatingBiSieve : virtual public Sieve {

  private:

    double ratio;

  public:

    AlternatingBiSieve(double ratio);
    double get_size() const;
    double get_mean() const;

};

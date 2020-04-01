#pragma once

#include "vec.hpp"
#include "particle.hpp"

class ParticleArrangement {

  private:

  public:

    ParticleArrangement() {}
    virtual ~ParticleArrangement() {};

    virtual bool check_interacts_with(const Particle *p) =0;
    virtual Vec3 get_interaction_result(const Particle *p) =0;
    virtual double get_sort_distance(const Particle *p) =0;
    virtual bool is_final() =0;
};


class ArrangementComparator {

  private:

    Particle *p;

  public:

    ArrangementComparator(Particle *p)
    {
      this->p = p;
    }

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_sort_distance(this->p) < r->get_sort_distance(this->p);
    }

};
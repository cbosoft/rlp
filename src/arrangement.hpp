#pragma once

#include <vector>

#include "vec.hpp"
#include "particle.hpp"
#include "exception.hpp"

class ParticleArrangement {

  private:

  public:

    ParticleArrangement() {}
    virtual ~ParticleArrangement() {};

    virtual bool check_interacts_with(const Particle *p) =0;
    virtual Vec3 get_interaction_result(const Particle *p) =0;
    virtual double get_sort_distance(const Particle *p) =0;
    virtual double get_z_position() =0;
    virtual bool covers(ParticleArrangement *arr) =0;
    virtual std::vector<Vec3> get_extents() =0;


    virtual bool is_final() =0;
    virtual std::string get_type() =0;
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


class ArrangementByNameComparator {

  public:

    int arrnameindex(ParticleArrangement *p)
    {
      int pi;
      std::string t = p->get_type();
      const char *c = t.c_str();
      if (strcmp(c, "Vertex") == 0) {
        pi = 0;
      }
      else if (strcmp(c, "Line") == 0) {
        pi = 1;
      }
      else if (strcmp(c, "Triangle") == 0) {
        pi = 2;
      }
      else {
        throw TypeError(Formatter() << "Unknown arrangement type: " << t << ".");
      }

      return pi;
    }

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      int li = this->arrnameindex(l), ri = this->arrnameindex(r);
      return li<ri;
    }
};

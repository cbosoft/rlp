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
    virtual double get_max_distance(const Particle *p) =0;
    virtual double get_min_distance(const Particle *p) =0;
    virtual bool covers(ParticleArrangement *arr) =0;
    virtual std::vector<Vec3> get_extents() =0;
    virtual std::string repr() =0;
    virtual Vec3 get_centre() const =0;

    virtual bool is_final() =0;
    virtual std::string get_type() =0;
    virtual int get_complexity() = 0;
};


class ArrangementComparator {

  private:

    const Particle *p;

  public:

    ArrangementComparator(const Particle *p)
    {
      this->p = p;
    }

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_sort_distance(this->p) < r->get_sort_distance(this->p);
    }

};

class ArrangementByComplexityComparator {

  public:

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_complexity() < r->get_complexity();
    }
};

class ArrangementByZPositionComparator {

  public:

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_z_position() < r->get_z_position();
    }
};

class ArrangementByMaxDistanceComparator {

  private:

    const Particle *p;

  public:

    ArrangementByMaxDistanceComparator(const Particle *p)
    {
      this->p = p;
    }

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_max_distance(this->p) < r->get_max_distance(this->p);
    }

};

class ArrangementByMinDistanceComparator {

  private:

    const Particle *p;

  public:

    ArrangementByMinDistanceComparator(const Particle *p)
    {
      this->p = p;
    }

    bool operator()(ParticleArrangement *l, ParticleArrangement *r)
    {
      return l->get_min_distance(this->p) < r->get_min_distance(this->p);
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

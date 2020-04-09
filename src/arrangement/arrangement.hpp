#pragma once

#include <vector>

#include "../util/vec.hpp"
#include "../particle/particle.hpp"
#include "../util/exception.hpp"

class ParticleArrangement {

  protected:

    bool get_two_particle_frictional_interaction(const Particle *settling, const Particle *settled, double &time, Vec3 &final_position) const
    {
      Vec3 dV = Vec3({0, 0, 1});
      Vec3 dP = settled->get_position() - settling->get_position();
      double A = dV.dot(dV);
      double B = 2.0*dV.dot(dP);
      double C = dP.dot(dP) - settled->get_radius() - settling->get_radius();
    
      double discriminant = (B*B) - (4.0*A*C);
      if (discriminant > 0.0) {
        double discriminant_root = std::pow(discriminant, 0.5);
    
        double bigsol = (-B + discriminant_root) / (2.0 * A);
        double lilsol = (-B - discriminant_root) / (2.0 * A);
        if ((bigsol > 1e-7) or (lilsol > 1e-7)) {
          // if results are not both approximately zero...
    
          if (lilsol < 0.0) {
            if (bigsol > 0.0) {
              time = bigsol;
            }
          }
          else {
            if (bigsol < 0.0) {
              time = lilsol;
            }
            else {
              time = lilsol;
              if (bigsol < lilsol) {
                time = bigsol;
              }
            }
          }
        }

        final_position = settling->get_position() + (Vec3({0, 0, -1})*time);
        return true;
      }
   
     return false;
    }

  public:

    ParticleArrangement() {}
    virtual ~ParticleArrangement() {};

    virtual bool check_interacts_with(const Particle *p) =0;
    virtual Vec3 get_interaction_result(const Particle *p) =0;
    virtual Vec3 get_frictional_interaction_result(const Particle *p) =0;
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

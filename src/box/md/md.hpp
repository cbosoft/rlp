#pragma once

#include "../box.hpp"

class MDBox : public virtual GenericBox {


  private:

    double solvent_viscosity;

    void set_particle_forces();
    void timestep();

  public:

    MDBox(double length, int verbosity, double friction_thresh);
    ~MDBox();

    void add_particle(Particle *p) override;
    void finish() override;

};

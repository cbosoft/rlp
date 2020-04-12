#pragma once
#include "../box.hpp"

class MonteCarloBox : public virtual GenericBox {

  public:

    MonteCarloBox();
    MonteCarloBox(double L, int verbosity=1, double friction_thresh=2.0) : GenericBox(L, verbosity, friction_thresh) {}

    void add_particle(Particle *p) override;
};

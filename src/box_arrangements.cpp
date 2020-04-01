#include <cmath>

#include "coords.hpp"
#include "box.hpp"
#include "vertex.hpp"
#include "line.hpp"

void PeriodicBox::update_arrangements()
{
  int N = this->particles.size();
  Particle *pi = this->particles[N-1];

  this->arrangements.push_back(new Vertex(pi, this));

  for (int j = 0; j < N-1; j++) {
    Particle *pj = this->particles[j];
    Vec3 rij = this->get_effective_separation(pi->get_position(), pj->get_position());

    if (rij.magnitude() > (pi->get_radius() + pj->get_radius() + 1.0)) {
      // too wide
      //std::cerr << "no because too wide" << std::endl;
      continue;
    }

    double theta = rij.angle_between(Z_AXIS);
    if ((theta < M_PI_4) or (theta > 3.0*M_PI_4)) {
      // too steep
      //std::cerr << "no because too steep" << std::endl;
      continue;
    }

    this->arrangements.push_back(new Line(pi, pj, this));

    // for (int k = 0; k < j; k++) {
    //   Particle *pk = this->particles[j];
    // }
  }
}

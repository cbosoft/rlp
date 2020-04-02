#include <cmath>

#include "coords.hpp"
#include "box.hpp"
#include "vertex.hpp"
#include "line.hpp"
#include "triangle.hpp"
#include "epsilon.hpp"


void PeriodicBox::update_arrangements()
{
  int N = this->particles.size();
  Particle *pi = this->particles[N-1];

  this->arrangements.push_back(new Vertex(pi, this));

  for (int j = 0; j < N-1; j++) {
    Particle *pj = this->particles[j];

    Vec3 rij = this->get_effective_separation(pi->get_position(), pj->get_position());
    double dist_ij = rij.magnitude();

    // check for, and add particle neighbours here - to be used to calculate top
    // layer of particles, removing arrangements below this line.
    if (FLOAT_EQ(dist_ij, (pi->get_radius() + pj->get_radius()) )) {
      pi->add_neighbour(pj);
      pj->add_neighbour(pi);
    }
    else if (dist_ij > (pi->get_radius() + pj->get_radius() + 1.0)) {
      //std::cerr << "no because too wide" << std::endl;
      this->log("Line disallowed: too wide.", 2);
      continue;
    }

    double theta = rij.angle_between(Z_AXIS);
    if ((theta < M_PI_4) or (theta > 3.0*M_PI_4)) {
      this->log(Formatter() << "Line disallowed: too steep (" << theta << "<" << M_PI_4 << "|" << theta << ">" << (3.0*M_PI_4) << ")", 2);
      continue;
    }

    this->arrangements.push_back(new Line(pi, pj, this));

    for (int k = 0; k < j; k++) {
      Particle *pk = this->particles[j];
      Vec3 rik = this->get_effective_separation(pi->get_position(), pk->get_position());
      if (rik.magnitude() > (pi->get_radius() + pj->get_radius() + 1.0)) {
        this->log("Triangle disallowed: too wide", 2);
        continue;
      }

      Vec3 rjk = this->get_effective_separation(pj->get_position(), pk->get_position());
      if (rjk.magnitude() > (pj->get_radius() + pj->get_radius() + 1.0)) {
        this->log("Triangle disallowed: too wide", 2);
        continue;
      }

      double theta = rik.angle_between(Z_AXIS);
      if ((theta < M_PI_4) or (theta > 3.0*M_PI_4)) {
        this->log("Triangle disallowed: too steep", 2);
        continue;
      }

      this->arrangements.push_back(new Triangle(pi, pj, pk, this));
    }
  }
}

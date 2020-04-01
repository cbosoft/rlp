#include "box.hpp"
#include "vertex.hpp"

void PeriodicBox::update_arrangements()
{
  int N = this->particles.size();
  Particle *pi = this->particles[N-1];

  this->arrangements.push_back(new Vertex(pi, this));

  // for (int j = 0; j < N-1; j++) {
  //   Particle *pj = this->particles[j];
  //   for (int k = 0; k < j; k++) {
  //     Particle *pk = this->particles[j];
  //   }
  // }
}

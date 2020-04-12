#include <cmath>
#include <algorithm>

#include "xyperiodicbox.hpp"
#include "../../arrangement/vertex.hpp"
#include "../../arrangement/line.hpp"
#include "../../arrangement/triangle.hpp"
#include "../../util/coords.hpp"
#include "../../util/epsilon.hpp"
#include "../../util/exception.hpp"


void XYPeriodicBox::update_arrangements()
{

  // first remove arrangements which are now blocked
  // double lowest_surface_height = this->get_lowest_surface_height();
  // for (auto it = this->arrangements.begin(); it != this->arrangements.end(); it++) {
  //   ParticleArrangement *arrangement = (*it);
  //   if (arrangement->get_z_position() <= lowest_surface_height)
  //     this->arrangements.erase(it++);
  // }
  


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
      this->log("Line disallowed: too wide.\n", 3);
      continue;
    }
    // else if (dist_ij < (pi->get_radius() + pj->get_radius())) {

    //   //this->particles.pop_back();

    //   std::string s = Formatter() << "Particle seperation less than total radii: "
    //       << pi->get_position() << " (" << pi->get_radius() << ") and "
    //       << pj->get_position() << " (" << pj->get_radius() << "): "
    //       << (pi->get_position() - pj->get_position()).magnitude() << " "
    //       << dist_ij << " < " << (pi->get_radius() + pj->get_radius()) << ".";
    //   this->log(s, 2);
    //   throw IntersectionError(s);
    // }

    // double theta = rij.angle_between(Z_AXIS);
    // if ((theta < M_PI_4) or (theta > 3.0*M_PI_4)) {
    //   this->log(Formatter() << "Line disallowed: too steep (" << theta << "<" << M_PI_4 << "|" << theta << ">" << (3.0*M_PI_4) << ")\n", 3);
    //   continue;
    // }

    this->arrangements.push_back(new Line(pi, pj, this));

    for (int k = 0; k < j; k++) {
      Particle *pk = this->particles[k];
      Vec3 rik = this->get_effective_separation(pi->get_position(), pk->get_position());
      if (rik.magnitude() > (pi->get_radius() + pj->get_radius() + 1.0)) {
        this->log("Triangle disallowed: too wide.\n", 3);
        continue;
      }

      Vec3 rjk = this->get_effective_separation(pj->get_position(), pk->get_position());
      if (rjk.magnitude() > (pj->get_radius() + pj->get_radius() + 1.0)) {
        this->log("Triangle disallowed: too wide.\n", 3);
        continue;
      }

      double theta = rik.angle_between(Z_AXIS);
      // if ((theta < M_PI_4) or (theta > 3.0*M_PI_4)) {
      //   this->log("Triangle disallowed: too steep.\n", 3);
      //   continue;
      // }

      theta = rik.angle_between(rij);
      if (theta > M_PI_2) {
        this->log(Formatter() << "Triangle disallowed: reflex (" << theta << ").\n", 3);
        continue;
      }
      else if (theta < (M_PI*0.1)) {
        this->log("Triangle disallowed: ~parallel.\n", 3);
        continue;
      }

      this->arrangements.push_back(new Triangle(pi, pj, pk, this));
    }
  }

}


ParticleArrangement *XYPeriodicBox::sort_and_filter_arrangements(std::list<ParticleArrangement *> &arrangements, const Particle *p) const
{

  // Filter by closest.
  ParticleArrangement *mindist = (*std::min_element(arrangements.begin(), arrangements.end(), ArrangementByMinDistanceComparator(p)));

  if (mindist == nullptr) {
    throw AuthorError(Formatter() << "Empty arrangements passed to sort_and_filter!");
  }


  double mindist_value = mindist->get_min_distance(p);
  std::list<ParticleArrangement *> arrangements_mindist;
  for (auto arrangement : arrangements) {
    if (arrangement->get_min_distance(p) == mindist_value) {
      arrangements_mindist.push_back(arrangement);
    }
  }

  // Filter for most complex.
  ParticleArrangement *maxcomplexity = (*std::max_element(arrangements_mindist.begin(), arrangements_mindist.end(), ArrangementByComplexityComparator()));
  int max_complexity = maxcomplexity->get_complexity();
  std::list<ParticleArrangement *> arrangements_maxcomplexity;
  for (auto arrangement : arrangements_mindist) {
    if (arrangement->get_complexity() == max_complexity) {
      arrangements_maxcomplexity.push_back(arrangement);
    }
  }

  // Finally, filter for closest at extremes.
  ParticleArrangement *minextremes = (*std::min_element(arrangements_maxcomplexity.begin(), arrangements_maxcomplexity.end(), ArrangementByMaxDistanceComparator(p)));
  return minextremes;
}

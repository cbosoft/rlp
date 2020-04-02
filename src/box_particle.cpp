#include <list>

#include "box.hpp"
#include "exception.hpp"

void PeriodicBox::add_particle(Particle *p)
{
  this->settle_particle(p);

  // TODO check OOB
  // TODO check intersection

  if (not p->is_settled()) {
    throw SettleError("Particle settle failed.");
  }

  this->particles.push_back(p);
  this->update_arrangements();

}


void PeriodicBox::settle_particle(Particle *p, int n, int recursion_limit)
{

  std::cerr << this->arrangements.size() << " " << this->lowest_surface_height << " " << p->get_position() << "-->";

  if (n >= recursion_limit)
    throw RecursionError("Exceeded recursion depth!");

  std::list<ParticleArrangement *> interacting_arrangements;
  for (ParticleArrangement *arrangement : this->arrangements) {
    if (arrangement->check_interacts_with(p))
      interacting_arrangements.push_back(arrangement);
  }

  if (interacting_arrangements.size() == 0) {
    p->set_z(0.0);
    p->set_settled();
    std::cerr << p->get_position() << "(FLOOR)!" << std::endl;
    return;
  }

  ArrangementComparator comparator(p);
  interacting_arrangements.sort(comparator);

  ParticleArrangement *interacting_arrangement = interacting_arrangements.front();

  std::cerr << interacting_arrangement->get_type();

  Vec3 new_position = interacting_arrangement->get_interaction_result(p);
  new_position = this->get_effective_position(new_position);
  p->set_position(new_position);

  if (interacting_arrangement->is_final()) {
    // TODO remove this arrangement from main list of
    std::cerr << p->get_position() << "!" << std::endl;
    p->set_settled();
  }
  else {
    this->settle_particle(p, n+1, recursion_limit);
  }
}


double PeriodicBox::get_lowest_surface_height(double thresh)
{

  if (this->particles.size() == 0)
    return -thresh;

  const Particle *lowest_surface_particle = this->particles[0];
  double lowest_surface_height = lowest_surface_particle->get_position().get(2);

  for (const Particle *p : this->particles) {
    double pz = p->get_position().get(2);
    if (p->neighbours.size()) {
      bool summit = true;
      for (const Particle *neighbour : p->neighbours) {
        if (neighbour->get_position().get(2) > pz) {
          summit = false;
          break;
        }
      }

      if (!summit)
        continue;
    }

    if (pz < lowest_surface_height) {
      lowest_surface_particle = p;
      lowest_surface_height = pz;
    }

  }

  this->lowest_surface_height =  lowest_surface_height - thresh;
  return this->lowest_surface_height;
}

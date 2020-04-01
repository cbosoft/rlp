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
    return;
  }

  ArrangementComparator comparator(p);
  interacting_arrangements.sort(comparator);
  ParticleArrangement *interacting_arrangement = interacting_arrangements.front();

  Vec3 new_position = interacting_arrangement->get_interaction_result(p);
  new_position = this->get_effective_position(new_position);
  p->set_position(new_position);

  if (interacting_arrangement->is_final()) {
    p->set_settled();
  }
  else {
    this->settle_particle(p, n+1, recursion_limit);
  }
}

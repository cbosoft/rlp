#include <list>

#include "xyperiodicbox.hpp"
#include "../../util/exception.hpp"
#include "../../util/random.hpp"


void XYPeriodicBox::add_particle(Particle *p)
{

  this->settle_particle(p);

  if (p->is_settled()) {
    this->particles.push_back(p);
    this->update_arrangements();

    // if (this->particles_are_seed) {
    //   ParticleArrangement *arr = (*select_randomly<>(this->arrangements.begin(), this->arrangements.end())); //(*std::max_element(this->arrangements.begin(), this->arrangements.end(), ArrangementByComplexityComparator()));
    //   Vec3 arr_centre = arr->get_centre();
    //   arr_centre.set(2, 10.0);
    //   this->add_particle(new Particle(1.0, arr_centre));
    // }


  }
  else {
    delete p;
  }

}


void XYPeriodicBox::check_particle_set_settled(Particle *p)
{
  bool intersected = false;
  Vec3 position = p->get_position();
  for (auto it = this->particles.rbegin(); it != this->particles.rend(); it++) {
    auto particle = *it;

    Vec3 dr = this->get_effective_separation(position, particle->get_position());
    double dist = dr.magnitude();
    if (dist+EPSILON < (particle->get_radius() + p->get_radius()))  {
      intersected = true;
      break;
    }

  }

  if (!intersected) {
    p->set_settled();
    this->log(Formatter() << p->get_position() << FG_GREEN "SETTLED!" RESET "\n");
  }
  else {
    this->log(BG_RED "INTERSECTED" RESET "\n");

    auto parr = p->get_previous_interacting();
    if ((parr != nullptr) and (parr->get_complexity() == 3)) {
      this->arrangements.remove(p->get_previous_interacting());
      delete p->get_previous_interacting();
      p->set_previous_interacting(nullptr);
    }

    //throw IntersectionError(Formatter() << "Particle " << p->repr() << " intersected");
  }
}


void XYPeriodicBox::settle_particle(Particle *p, int n, int recursion_limit)
{

  this->log(Formatter() << p->get_position() << "->");

  if (n >= recursion_limit)
    throw RecursionError("Exceeded recursion depth!");

  std::list<ParticleArrangement *> interacting_arrangements;
  for (ParticleArrangement *arrangement : this->arrangements) {
    if (arrangement->check_interacts_with(p))
      interacting_arrangements.push_back(arrangement);
  }

  if (interacting_arrangements.size() == 0) {
    p->set_z(0.0);
    this->check_particle_set_settled(p);
    return;
  }

  ParticleArrangement *interacting_arrangement = this->sort_and_filter_arrangements(interacting_arrangements, p);

  this->log(Formatter() << interacting_arrangement->repr() << "->");


  if (this->is_particle_frictional(p)) {

    Vec3 new_position = interacting_arrangement->get_frictional_interaction_result(p);
    p->set_position(new_position);
    p->set_previous_interacting(interacting_arrangement);
    this->check_particle_set_settled(p);

  }
  else{

    Vec3 new_position = interacting_arrangement->get_interaction_result(p);
    new_position = this->get_effective_position(new_position);
    p->set_position(new_position);
    p->set_previous_interacting(interacting_arrangement);

    if (interacting_arrangement->is_final()) {


      this->check_particle_set_settled(p);


      // for (auto arrangement : interacting_arrangements) {

      //   if (interacting_arrangement == arrangement)
      //     continue;

      //   if (interacting_arrangement->covers(arrangement)) {
      //     this->arrangements.remove(arrangement);
      //     delete arrangement;
      //   }
      // }

      // this->arrangements.remove(interacting_arrangement);
      // delete interacting_arrangement;
    }
    else {
      this->settle_particle(p, n+1, recursion_limit);
    }
  }
}


void XYPeriodicBox::add_particle_no_settle(Particle *p)
{
  this->particles.push_back(p);
}

#include "multi.hpp"


MultiInteractionTest::MultiInteractionTest(bool is_quiet) 
  : TestRunner("Multi particle test", is_quiet)
{
  this->input_data = {
    Vec3({4.22479, 1.67952, 10.0})
  };
  this->expected_results = {
    Vec3({4.0653970383976529845, 1.3030129329153037343, 0.77670979214002822122})
  };
}


void MultiInteractionTest::run(Vec3 point, Vec3 expected_result)
{

  // build
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, Vec3({4.71171, 0.118055, 10.0}));
  box.add_particle(pi);
  pi->set_position(Vec3({4.71171, 0.118055, 0.363677}));

  Particle *pj = new Particle(1.0, Vec3({3.52381, 1.62458, 10.0}));
  box.add_particle(pj);

  Particle *pk = new Particle(1.0, Vec3({4.51687, 1.74221, 10.0}));
  box.add_particle(pk);

  Particle *pl = new Particle(1.0, Vec3({4.05131, 0.775026, 10.0}));
  box.add_particle(pl);

  Particle *pm = new Particle(1.0, point);
  box.add_particle(pm);


  Vec3 result = pm->get_position();
  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter() << "Particle did not settle to expected position. Expected " << expected_result << ", got " << result << ".");
  }

}

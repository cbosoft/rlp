#include "line.hpp"


LineInteractionTest::LineInteractionTest(bool is_quiet) 
  : TestRunner("Line test (interaction)", is_quiet)
{
  this->input_data = {
    Vec3({5.0, 5.1, 10.0}),
    Vec3({5.0, 4.9, 10.0}),
    Vec3({5.0, 5.0, 10.0}) // this lands exactly on the line, it should handle it and shoogle the point randomly (it ends up going down, the seed is set in test)
  };
  this->expected_results = {
    Vec3({5.0, 5.8660254037844383745, 0.0}),
    Vec3({5.0, 4.1339745962155616255, 0.0}),
    Vec3({5.0, 4.1339745962155616255, 0.0})
  };
}


void LineInteractionTest::run(Vec3 point, Vec3 expected_result)
{
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, Vec3({4.5, 5.0, 0.0})),
    *pj = new Particle(1.0, Vec3({5.5, 5.0, 0.0}));
  Line line(pi, pj, &box);
  box.add_particle(pi);
  box.add_particle(pj);

  seed(1);

  if (box.get_number_arrangements() != 3)
    this->fail(Formatter() << "Incorrect number of arrangments. Got " << box.get_number_arrangements() << ", expected 3.");

  Particle *pk = new Particle(1.0, point);
  bool interacts = line.check_interacts_with(pk);
  if (!interacts) {
    this->fail(Formatter() 
        << "Particle " << pk->get_position() 
        << " should interact with Line " << line.repr() << ", but didn't.");
  }

  box.add_particle(pk);
  Vec3 result = pk->get_position();

  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter() << "Particle's final position was not " << expected_result << " as expected, but was " << result << ".");
  }

}

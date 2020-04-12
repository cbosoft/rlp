#include "line.hpp"


LineCreationTest::LineCreationTest(bool is_quiet) 
  : TestRunner("Line test (creation)", is_quiet)
{
  this->input_data = {
    std::make_pair(Vec3({4.5, 5.0, 0.0}), Vec3({5.5, 5.0, 0.0}))
  };
  this->expected_results = {
    3
  };
}


void LineCreationTest::run(std::pair<Vec3, Vec3> pair, int expected_result)
{
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, pair.first),
    *pj = new Particle(1.0, pair.second);
  box.add_particle(pi);
  box.add_particle(pj);
  int result = box.get_number_arrangements();

  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter() 
        << "Particles " << pi->get_position() << " and " << pj->get_position()
        << " should form " << expected_result << " arrangements, but got " << result << ".");
  }

}

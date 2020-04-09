#include "volume.hpp"


VolumeBasicTest::VolumeBasicTest(bool is_quiet)
  : TestRunner("Volume test (basic)", is_quiet)
{
  this->input_data = {
    {Vec3({5.0, 5.0, 5.0})},
    {Vec3({5.0, 5.0, 5.0}), Vec3({5.0, 7.0, 5.0})},
    {Vec3({5.0, 5.0, 5.0}), Vec3({5.0, 7.0, 5.0}), Vec3({7.0, 7.0, 5.0})},
  };
  this->expected_results = {
    M_PI/6.0,
    M_PI/3.0,
    M_PI/2.0,
  };
}


void VolumeBasicTest::run(std::vector<Vec3> input_data, double expected_result)
{
  PeriodicBox box(10.0, this->is_quiet?-10:0);

  double total = 0.0;
  for (auto point : input_data) {
    Particle *p = new Particle(1.0, point);
    box.add_particle_no_settle(p);
    total += box.get_volume_particle(p, 0.0, 10.0);
  }

  if (FLOAT_EQ(total, expected_result)) {
    this->pass();
  }
  else {
    this->fail(Formatter() << "Volume (basic) not correcly calculated. Expected " << expected_result << ", got " << total << ".");
  }
}

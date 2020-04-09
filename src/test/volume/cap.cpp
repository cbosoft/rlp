#include "volume.hpp"


VolumeCapTest::VolumeCapTest(bool is_quiet)
  : TestRunner("Volume test (cap)", is_quiet)
{
  this->input_data = {
    {Vec3({0.0, 0.0, 0.0})},
    {Vec3({0.0, 0.0, 0.0}), Vec3({0.0, 10.0, 0.0})},
    {Vec3({0.0, 0.0, 0.0}), Vec3({0.0, 10.0, 10.0}), Vec3({10.0, 10.0, 10.0})},
  };
  this->expected_results = {
    M_PI/12.0,
    M_PI/6.0,
    M_PI/4.0,
  };
}


void VolumeCapTest::run(std::vector<Vec3> input_data, double expected_result)
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
    this->fail(Formatter() << "Volume (cap) not correcly calculated. Expected " << expected_result << ", got " << total << ".");
  }
}

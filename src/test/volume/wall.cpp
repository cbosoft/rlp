#include "volume.hpp"


VolumeWallTest::VolumeWallTest(bool is_quiet)
  : TestRunner("Volume test (wall)", is_quiet)
{
  this->input_data = {
    {Vec3({0.0, 0.0, 5.0})},
    {Vec3({0.0, 0.0, 5.0}), Vec3({0.0, 10.0, 5.0})},
    {Vec3({0.0, 0.0, 5.0}), Vec3({0.0, 10.0, 5.0}), Vec3({10.0, 10.0, 5.0})},
  };
  this->expected_results = {
    M_PI/6.0,
    M_PI/3.0,
    M_PI/2.0,
  };
}


void VolumeWallTest::run(std::vector<Vec3> input_data, double expected_result)
{
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);

  double total = 0.0;
  for (auto point : input_data) {
    Particle *p = new Particle(1.0, point);
    box.add_particle_no_settle(p);
    total += box.get_volume_particle(p);
  }

  if (FLOAT_EQ(total, expected_result)) {
    this->pass();
  }
  else {
    this->fail(Formatter() << "Volume (wall) not correcly calculated. Expected " << expected_result << ", got " << total << ".");
  }
}

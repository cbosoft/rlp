#include "vertex.hpp"


VertexCreationTest::VertexCreationTest(bool is_quiet)
  : TestRunner("Vertex test (creation)", is_quiet)
{
  this->input_data = {
    Vec3({5.0, 5.0, 10.0})
  };
  this->expected_results = {
    1
  };
}


void VertexCreationTest::run(Vec3 point, int expected_result)
{
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *p = new Particle(1.0, point);
  box.add_particle(p);

  int result = box.get_number_arrangements();

  if (result != expected_result) {
    this->fail(Formatter() 
        << "Particle " << p->get_position() 
        << " should form "<< expected_result << "arrangements, but got " << result << " instead.");
  }
  else {
    this->pass();
  }

}

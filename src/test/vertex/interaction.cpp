#include "vertex.hpp"


VertexInteractionTest::VertexInteractionTest(bool is_quiet)
  : TestRunner("Vertex test (interaction)", is_quiet)
{
  this->input_data = {
    Vec3({5.0, 5.0, 10.0}),
    Vec3({4.5, 4.5, 10.0}),
    Vec3({5.0, 5.5, 10.0}),
    Vec3({4.0, 4.0, 10.0})
  };
  this->expected_results = {
    true,
    true,
    true,
    false
  };
}


void VertexInteractionTest::run(Vec3 point, bool expected_result)
{
  PeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, point);
  Vertex vertex(pi, &box);
  box.add_particle(pi);

  if (box.get_number_arrangements() != 1)
    this->fail("incorrect number of arrangments");

  Particle *pj = new Particle(1.0, Vec3({5.0, 5.0, 10.0}));

  bool result = vertex.check_interacts_with(pj);
  std::string n = expected_result ? "" : "n't", 
    nn = expected_result ? "n't" : "";

  if (result != expected_result) {
    Vec3 ri = pi->get_position();
    Vec3 rj = pj->get_position();
    Vec3 rij = box.get_effective_separation(ri, rj);
    std::cerr << rij.magnitude2() << std::endl;
    this->fail(Formatter() 
        << "Particle " << pj->get_position() 
        << " should" << n << " interact with Vertex " << pi->get_position() << ", but did" << nn << ".");
  }
  else {
    this->pass();
  }

  delete pj;
}

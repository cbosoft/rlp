#pragma once

#include "test.hpp"
#include "../vec.hpp"
#include "../vertex.hpp"
#include "../box.hpp"
#include "../particle.hpp"

class VertexInteractionTest : public virtual TestRunner<Vec3, bool> {


  public:
    VertexInteractionTest(int &counter) : TestRunner(counter, "Vertex Test (interaction)")
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

    void run(Vec3 point, bool expected_result) override
    {
      PeriodicBox box(10.0);
      Particle *pi = new Particle(1.0, point);
      Vertex vertex(pi, &box);
      box.add_particle(pi);

      if (box.get_number_arrangements() != 1)
        this->fail("incorrect number of arrangments");

      Particle *pj = new Particle(1.0, Vec3({5.0, 5.0, 10.0}));


      bool result = vertex.check_interacts_with(pj);

      if (result != expected_result) {
        Vec3 ri = pi->get_position();
        Vec3 rj = pj->get_position();
        Vec3 rij = box.get_effective_separation(ri, rj);
        std::cerr << rij.magnitude2() << std::endl;
        this->fail(Formatter() << "vertex/particle did not do as expected (" << ri.repr() << ", " << rj.repr() << " : " << rij.magnitude() << ")");
      }
      else {
        this->pass();
      }
    }
};

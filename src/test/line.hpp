#include "test.hpp"
#include "../line.hpp"
#include "../vec.hpp"

class LineCreationTest : public virtual TestRunner<std::pair<Vec3, Vec3>, int> {

  public:
    
    LineCreationTest(int &counter) 
      : TestRunner(counter, "Line test (creation)")
    {
      this->input_data = {
        std::make_pair(Vec3({4.5, 5.0, 0.0}), Vec3({5.5, 5.0, 0.0}))
      };
      this->expected_results = {
        3
      };
    }

    void run(std::pair<Vec3, Vec3> pair, int expected_result) override
    {
      PeriodicBox box(10.0, 0);
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
};


class LineInteractionTest : public virtual TestRunner<std::pair<Vec3, Vec3>, std::pair<bool, int>> {

  public:
    
    LineInteractionTest(int &counter) 
      : TestRunner(counter, "Line test (interaction)")
    {
      this->input_data = {
        std::make_pair(Vec3({4.5, 5.0, 0.0}), Vec3({5.5, 5.0, 0.0}))
      };
      this->expected_results = {
        std::make_pair(true, 3)
      };
    }

    void run(std::pair<Vec3, Vec3> pair, std::pair<bool, int> result_pair) override
    {
      Particle *pi = new Particle(1.0, pair.first),
        *pj = new Particle(1.0, pair.second);
      PeriodicBox box(10.0, 0);
      Line line(pi, pj, &box);
      box.add_particle(pi);
      box.add_particle(pj);

      if (box.get_number_arrangements() != result_pair.second)
        this->fail(Formatter() << "Incorrect number of arrangments. Got " << box.get_number_arrangements() << ", expected " << result_pair.second << ".");

      Particle *pk = new Particle(1.0, Vec3({5.0, 5.0, 10.0}));
      bool result = line.check_interacts_with(pk);
      bool expected_result = result_pair.first;
      std::string n = expected_result ? "" : "n't", 
        nn = expected_result ? "n't" : "";

      if (result == expected_result) {
        this->pass();
      }
      else {
        Vec3 ri = pi->get_position();
        Vec3 rj = pj->get_position();
        Vec3 rij = box.get_effective_separation(ri, rj);
        std::cerr << rij.magnitude2() << std::endl;
        this->fail(Formatter() 
            << "Particle " << pj->get_position() 
            << " should" << n << " interact with Line " << pi->get_position() << "--" << pj->get_position() << ", but did" << nn << ".");
      }

      delete pk;

    }
};

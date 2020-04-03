#include "test.hpp"
#include "../vec.hpp"
#include "../particle.hpp"
#include "../box.hpp"
#include "../line.hpp"
#include "../random.hpp"
#include "../exception.hpp"


class MultiInteractionTest : public virtual TestRunner<Vec3, Vec3> {

  public:
    
    MultiInteractionTest(int &counter) 
      : TestRunner(counter, "Line test (interaction)")
    {
      this->input_data = {
        Vec3({5.0, 5.1, 10.0})
      };
      this->expected_results = {
        Vec3({5.0, 5.8660254037844383745, 0.0})
      };
    }

    void run(Vec3 point, Vec3 expected_result) override
    {

      PeriodicBox box(10.0, 10);
      Particle *pi = new Particle(1.0, Vec3({4.71171, 0.118055, 10.0}));
      box.add_particle(pi);
      pi->set_position(Vec3({4.71171, 0.118055, 0.363677}));

      try{
      Particle *pj = new Particle(1.0, Vec3({3.52381, 1.62458, 10.0}));
      box.add_particle(pj);

      Particle *pk = new Particle(1.0, Vec3({4.51687, 1.74221, 10.0}));
      box.add_particle(pk);

      Particle *pl = new Particle(1.0, Vec3({4.05131, 0.775026, 10.0}));
      box.add_particle(pl);

      Particle *pm = new Particle(1.0, Vec3({4.22479, 1.67952, 10.0}));

      auto arrangements = box.get_arrangements();
      for (auto a : arrangements) {
        if (a->get_complexity() == 3)
          std::cerr << a->repr() << " " << a->get_min_distance(pm) << " " << a->get_max_distance(pm) << std::endl;
      }


        box.add_particle(pm);
      }
      catch (const Exception&) {
      }

      auto particles = box.get_particles();
      std::cout << box.get_L() << std::endl;
      for (auto p : particles) {
        std::cout << p->get_diameter() << ", " << p->get_position().to_csv() << std::endl;
      }

      (void)point;
      (void)expected_result;
    }

};

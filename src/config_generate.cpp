#include <chrono>

#include "random.hpp"
#include "vec.hpp"
#include "config.hpp"

#define CLOCK std::chrono::steady_clock

void ConfigGenerator::generate_particles(int n)
{

  double generation_duration = -1;
  for (int i = 0; i < n; i++) {
    std::cerr << "(" << generation_duration << ") " << i << " ";
    auto before = CLOCK::now();
    this->generate_particle();
    auto after = CLOCK::now();
    generation_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
  }
}

void ConfigGenerator::generate_particle()
{
  Vec3 position = vec3_urand(0.0, this->box.get_L());
  position.set(2, this->box.get_L());
  Particle *p = new Particle(1.0, position);
  this->box.add_particle(p);
}

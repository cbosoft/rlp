#include "random.hpp"
#include "vec.hpp"
#include "config.hpp"

void ConfigGenerator::generate_particles(int n)
{
  for (int i = 0; i < n; i++) {
    std::cerr << i << std::endl;
    this->generate_particle();
  }
}

void ConfigGenerator::generate_particle()
{
  Vec3 position = vec_urand(0.0, this->box.get_L());
  position.set(2, this->box.get_L());
  Particle *p = new Particle(1.0, position);
  this->box.add_particle(p);
}

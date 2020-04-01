#include "config.hpp"

int main(void)
{

  ConfigGenerator cg = ConfigGenerator(10.0);
  cg.generate_particles(1000);
}

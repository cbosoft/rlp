#include "config.hpp"
#include "test/run_tests.hpp"

int main(void)
{

  // TODO argument parsing

  run_tests();

  ConfigGenerator cg = ConfigGenerator(10.0);
  cg.generate_particles(1000);
}

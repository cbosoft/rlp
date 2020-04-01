#include "run_tests.hpp"
#include "vec.hpp"
#include "vertex.hpp"

void run_tests()
{
  int counter = 0;
  VectorMagnitudeTest(counter).run_each();
  VectorDotTest(counter).run_each();
  VectorCrossTest(counter).run_each();
  VectorOrthoComponentTest(counter).run_each();

  // vertex tests
  VertexInteractionTest(counter).run_each();
}

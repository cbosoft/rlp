#include "run_tests.hpp"
#include "vec.hpp"
#include "vertex.hpp"
#include "line.hpp"

void run_tests()
{
  int counter = 0;
  VectorMagnitudeTest(counter).run_each();
  VectorDotTest(counter).run_each();
  VectorCrossTest(counter).run_each();
  VectorOrthoComponentTest(counter).run_each();
  VectorAngleBetweenTest(counter).run_each();

  // vertex tests
  VertexCreationTest(counter).run_each();
  VertexInteractionTest(counter).run_each();

  // line tests
  LineCreationTest(counter).run_each();
  LineInteractionTest(counter).run_each();

}

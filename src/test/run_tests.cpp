#include "run_tests.hpp"
#include "vec.hpp"
#include "vertex.hpp"
#include "line.hpp"
#include "triangle.hpp"

void run_tests()
{
  int counter = 0;

  // vector tests
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

  // triangle tests
  TriangleTrilaterationTest(counter).run_each();
  TriangleCreationTest(counter).run_each();

  if (counter)
    throw TestError(Formatter() << counter << " tests failed!");
}

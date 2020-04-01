#include "vec.hpp"
#include "vertex.hpp"

int main(void)
{
  int counter = 0;
  VectorMagnitudeTest(counter).run_each();
  VectorCrossTest(counter).run_each();
  VectorOrthoComponentTest(counter).run_each();

  // vertex tests
  VertexInteractionTest(counter).run_each();
}

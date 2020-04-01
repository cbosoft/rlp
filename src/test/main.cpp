#include "vec.hpp"
#include "vertex.hpp"

int main(void)
{
  int counter = 0;
  VectorMagnitudeTest(counter).run_each();
  VertexInteractionTest(counter).run_each();
}

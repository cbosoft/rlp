#include "run_tests.hpp"
#include "vec/vec.hpp"
#include "vertex/vertex.hpp"
#include "line/line.hpp"
#include "triangle/triangle.hpp"
#include "multi/multi.hpp"
#include "volume/volume.hpp"

void run_tests(bool quiet)
{
  // vector tests
  VectorMagnitudeTest(quiet).run_each();
  VectorDotTest(quiet).run_each();
  VectorCrossTest(quiet).run_each();
  VectorOrthoComponentTest(quiet).run_each();
  VectorAngleBetweenTest(quiet).run_each();

  // vertex tests
  VertexCreationTest(quiet).run_each();
  VertexInteractionTest(quiet).run_each();

  // line tests
  LineCreationTest(quiet).run_each();
  LineInteractionTest(quiet).run_each();

  // triangle tests
  TriangleTrilaterationTest(quiet).run_each();
  TriangleCreationTest(quiet).run_each();

  // multi
  MultiInteractionTest(quiet).run_each();

  // volume
  VolumeBasicTest(quiet).run_each();
  VolumeWallTest(quiet).run_each();
  VolumeCapTest(quiet).run_each();
}

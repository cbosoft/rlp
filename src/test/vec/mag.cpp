#include "vec.hpp"


VectorMagnitudeTest::VectorMagnitudeTest(bool is_quiet) : TestRunner("Vector test (magnitude)", is_quiet)
{
  this->input_data = {
    Vec3({1.0, 0.0, 0.0}),
    Vec3({0.0, 1.0, 0.0}),
    Vec3({0.0, 0.0, 1.0}),
    Vec3({3.0, 4.0, 12.0})
  };

  this->expected_results = {
    1.0,
    1.0,
    1.0,
    13.0
  };
}

void VectorMagnitudeTest::run(Vec3 vector, double expected_result)
{
  double result = vector.magnitude();

  if (std::fabs(result-expected_result) < EPSILON) {
    this->pass();
  }
  else {
    this->fail(Formatter() << "Magnitude of " << vector.repr() << " calculated as " << result << " instead of " << expected_result << "." );
  }
}

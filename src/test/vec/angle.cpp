#include "vec.hpp"

VectorAngleBetweenTest::VectorAngleBetweenTest(bool is_quiet) 
  : TestRunner("Vector test (angle between)", is_quiet)
{
  this->input_data = {
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0})),
    std::make_pair(Vec3({1.0, 1.0, 0.0}), Vec3({0.0, 1.0, 0.0})),
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({-1.0, 0.0, 0.0})),
  };
  this->expected_results = {
    M_PI_2, M_PI_4, M_PI
  };
}

void VectorAngleBetweenTest::run(std::pair<Vec3, Vec3> pair, double expected_result)
{
  Vec3 left_vector = pair.first, right_vector = pair.second;

  double result = left_vector.angle_between(right_vector);

  if (FLOAT_EQ(result, expected_result)) {
    this->pass();
  }
  else {
    this->fail(Formatter()
        << "Angle between " << left_vector
        << " and " << right_vector
        << " calculated as " << result
        << " instead of " << expected_result << "." );
  }
}

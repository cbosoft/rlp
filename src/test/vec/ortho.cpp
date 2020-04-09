#include "vec.hpp"


VectorOrthoComponentTest::VectorOrthoComponentTest(bool is_quiet) 
  : TestRunner("Vector test (orthogonal component)", is_quiet)
{
  this->input_data = {
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0})),
    std::make_pair(Vec3({5.0, 3.0, 2.0}), Vec3({0.0, 1.0, 0.0})),
    std::make_pair(Vec3({1.0, 1.0, 1.0}), Vec3({0.0, 0.0, 1.0}))
  };
  this->expected_results = {
    Vec3({0.0, 0.0, 0.0}),
    Vec3({0.0, 3.0, 0.0}),
    Vec3({0.0, 0.0, 1.0})
  };
}


void VectorOrthoComponentTest::run(std::pair<Vec3, Vec3> pair, Vec3 expected_result)
{
  Vec3 left_vector = pair.first, right_vector = pair.second;

  Vec3 result = left_vector.component_along(right_vector);

  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter()
        << "Component of " << left_vector
        << " along " << right_vector
        << " calculated as " << result
        << " instead of " << expected_result << "." );
  }
}

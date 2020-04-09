#include "vec.hpp"


VectorDotTest::VectorDotTest(bool is_quiet) 
  : TestRunner("Vector test (scalar multiplication)", is_quiet)
{
  this->input_data = {
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 0.0, 0.0})),
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0})),
    std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0}))
  };
  this->expected_results = {
    0.0, 1.0, 0.0
  };
}


void VectorDotTest::run(std::pair<Vec3, Vec3> pair, double expected_result)
{
  Vec3 left_vector = pair.first, right_vector = pair.second;

  double result = left_vector.dot(right_vector);

  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter() 
        << "Magnitude of " 
        << left_vector << " · " << right_vector
        << " calculated as " << result
        << " instead of " << expected_result << "." );
  }
}

#pragma once

#include "test.hpp"
#include "../vec.hpp"
#include "../epsilon.hpp"

class VectorMagnitudeTest : public virtual TestRunner<double> {

  public:
    VectorMagnitudeTest(int &counter) 
      : TestRunner(counter, "Vector test (magnitude)")
    {
      this->points = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {3.0, 4.0, 12.0}
      };
      this->expected_results = {
        1.0,
        1.0,
        1.0,
        13.0
      };
    }

    void run(int test_index) override
    {
      Vec3 v(this->points[test_index]);

      double result = v.magnitude();
      double expected_result = this->expected_results[test_index];

      if (std::fabs(result-expected_result) < EPSILON) {
        this->pass();
      }
      else {
        this->fail(Formatter() << "Magnitude of " << v.repr() << " calculated as " << result << " instead of " << expected_result << "." );
      }
    }
};

#pragma once

#include "test.hpp"
#include "../vec.hpp"
#include "../epsilon.hpp"

class VectorMagnitudeTest : public virtual TestRunner<Vec3, double> {

  public:
    VectorMagnitudeTest(int &counter) 
      : TestRunner(counter, "Vector test (magnitude)")
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

    void run(Vec3 vector, double expected_result) override
    {
      double result = vector.magnitude();

      if (std::fabs(result-expected_result) < EPSILON) {
        this->pass();
      }
      else {
        this->fail(Formatter() << "Magnitude of " << vector.repr() << " calculated as " << result << " instead of " << expected_result << "." );
      }
    }
};


class VectorCrossTest : public virtual TestRunner<std::pair<Vec3, Vec3>, Vec3> {

  public:
    VectorCrossTest(int &counter) 
      : TestRunner(counter, "Vector test (cross multiplication)")
    {
      this->input_data = {
        std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 0.0, 0.0})),
        std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0})),
        std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0}))
      };
      this->expected_results = {
        Vec3({0.0, 0.0, 0.0}),
        Vec3({0.0, 0.0, 0.0}),
        Vec3({0.0, 0.0, 1.0})
      };
    }

    void run(std::pair<Vec3, Vec3> pair, Vec3 expected_result) override
    {
      Vec3 left_vector = pair.first, right_vector = pair.second;

      Vec3 result = left_vector.cross(right_vector);

      if (result == expected_result) {
        this->pass();
      }
      else {
        this->fail(Formatter() 
            << "Magnitude of " 
            << left_vector << " ✕ " << right_vector
            << " calculated as " << result
            << " instead of " << expected_result << "." );
      }
    }
};


class VectorOrthoComponentTest : public virtual TestRunner<std::pair<Vec3, Vec3>, Vec3> {

  public:
    VectorOrthoComponentTest(int &counter) 
      : TestRunner(counter, "Vector test (orthogonal component)")
    {
      this->input_data = {
        std::make_pair(Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0}))
      };
      this->expected_results = {
        Vec3({0.0, 0.0, 0.0})
      };
    }

    void run(std::pair<Vec3, Vec3> pair, Vec3 expected_result) override
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
};
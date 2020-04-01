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


class VectorDotTest : public virtual TestRunner<std::pair<Vec3, Vec3>, double> {

  public:
    VectorDotTest(int &counter) 
      : TestRunner(counter, "Vector test (scalar multiplication)")
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

    void run(std::pair<Vec3, Vec3> pair, double expected_result) override
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


class VectorAngleBetweenTest : public virtual TestRunner<std::pair<Vec3, Vec3>, double> {

  public:
    VectorAngleBetweenTest(int &counter) 
      : TestRunner(counter, "Vector test (angle between)")
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

    void run(std::pair<Vec3, Vec3> pair, double expected_result) override
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
};

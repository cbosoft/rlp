#pragma once

#include "../test.hpp"
#include "../../util/vec.hpp"
#include "../../util/epsilon.hpp"

class VectorMagnitudeTest : public virtual TestRunner<Vec3, double> {

  public:
    VectorMagnitudeTest(bool is_quiet);
    void run(Vec3 vector, double expected_result) override;
};


class VectorDotTest : public virtual TestRunner<std::pair<Vec3, Vec3>, double> {

  public:
    VectorDotTest(bool is_quiet);
    void run(std::pair<Vec3, Vec3> pair, double expected_result) override;
};


class VectorCrossTest : public virtual TestRunner<std::pair<Vec3, Vec3>, Vec3> {

  public:
    VectorCrossTest(bool is_quiet);
    void run(std::pair<Vec3, Vec3> pair, Vec3 expected_result) override;
};


class VectorOrthoComponentTest : public virtual TestRunner<std::pair<Vec3, Vec3>, Vec3> {

  public:
    VectorOrthoComponentTest(bool is_quiet);
    void run(std::pair<Vec3, Vec3> pair, Vec3 expected_result) override;
};


class VectorAngleBetweenTest : public virtual TestRunner<std::pair<Vec3, Vec3>, double> {

  public:
    VectorAngleBetweenTest(bool is_quiet);
    void run(std::pair<Vec3, Vec3> pair, double expected_result) override;
};

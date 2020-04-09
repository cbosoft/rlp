#pragma once

#include "../test.hpp"
#include "../../box/box.hpp"
#include "../../arrangement/triangle.hpp"
#include "../../util/epsilon.hpp"


class TriangleTrilaterationTest : public virtual TestRunner<std::pair<std::array<Vec3, 3>, double>, std::pair<bool, Vec3>> {

  public:

    TriangleTrilaterationTest(bool is_quiet);
    void run(std::pair<std::array<Vec3, 3>, double> points_and_radius, std::pair<bool, Vec3> expected_result_pair) override;
};


class TriangleCreationTest : public virtual TestRunner<std::array<Vec3, 3>, std::vector<std::string>> {

  public:

    TriangleCreationTest(bool is_quiet);
    void run(std::array<Vec3, 3> points, std::vector<std::string> expected_result) override;
};

#pragma once

#include "../test.hpp"
#include "../../particle/particle.hpp"
#include "../../box/xyperiodic/xyperiodicbox.hpp"
#include "../../util/vec.hpp"
#include "../../util/epsilon.hpp"


class VolumeBasicTest : public virtual TestRunner<std::vector<Vec3>, double> {

  public:

    VolumeBasicTest(bool is_quiet);
    void run(std::vector<Vec3> input_data, double expected_result) override;

};


class VolumeWallTest : public virtual TestRunner<std::vector<Vec3>, double> {

  public:

    VolumeWallTest(bool is_quiet);
    void run(std::vector<Vec3> input_data, double expected_result) override;

};


class VolumeCapTest : public virtual TestRunner<std::vector<Vec3>, double> {

  public:

    VolumeCapTest(bool is_quiet);
    void run(std::vector<Vec3> input_data, double expected_result) override;

};

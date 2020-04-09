#pragma once

#include "../test.hpp"
#include "../../util/vec.hpp"
#include "../../arrangement/vertex.hpp"
#include "../../box/box.hpp"
#include "../../particle/particle.hpp"


class VertexCreationTest : public virtual TestRunner<Vec3, int> {

  public:

    VertexCreationTest(bool is_quiet);
    void run(Vec3 point, int expected_result) override;

};


class VertexInteractionTest : public virtual TestRunner<Vec3, bool> {

  public:

    VertexInteractionTest(bool is_quiet);
    void run(Vec3 point, bool expected_result) override;

};

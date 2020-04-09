#include "../test.hpp"
#include "../../arrangement/line.hpp"
#include "../../util/vec.hpp"
#include "../../util/random.hpp"


class LineCreationTest : public virtual TestRunner<std::pair<Vec3, Vec3>, int> {

  public:
    
    LineCreationTest(bool is_quiet);
    void run(std::pair<Vec3, Vec3> pair, int expected_result) override;
};


class LineInteractionTest : public virtual TestRunner<Vec3, Vec3> {

  public:
    
    LineInteractionTest(bool is_quiet);
    void run(Vec3 point, Vec3 expected_result) override;
};

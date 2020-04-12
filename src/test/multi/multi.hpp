#include "../test.hpp"
#include "../../util/vec.hpp"
#include "../../particle/particle.hpp"
#include "../../box/xyperiodic/xyperiodicbox.hpp"
#include "../../arrangement/line.hpp"
#include "../../util/random.hpp"
#include "../../util/exception.hpp"


class MultiInteractionTest : public virtual TestRunner<Vec3, Vec3> {

  public:

    MultiInteractionTest(bool is_quiet);
    void run(Vec3 point, Vec3 expected_result) override;

};

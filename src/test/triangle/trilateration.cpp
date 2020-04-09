#include "triangle.hpp"

TriangleTrilaterationTest::TriangleTrilaterationTest(bool is_quiet)
  : TestRunner("Triangle test (trilateration)", is_quiet)
{
  this->input_data = {
    std::make_pair(std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0})}), 0.5),
    std::make_pair(std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0})}), 0.1),
    std::make_pair(std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.3}), Vec3({0.0, 1.0, 0.4})}), 0.5)
  };
  this->expected_results = {
    std::make_pair(true, Vec3({0.5, 0.5, std::pow(0.5, 0.5)})),
    std::make_pair(false, Vec3({0.0, 0.0, 0.0})),
    std::make_pair(true, Vec3({0.26192260205880824042, 0.20256346941174424581, 0.94359132647063925781}))
  };
}

void TriangleTrilaterationTest::run(std::pair<std::array<Vec3, 3>, double> points_and_radius, std::pair<bool, Vec3> expected_result_pair)
{
  auto points = points_and_radius.first;
  double radius = points_and_radius.second;
  PeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, points[0]),
    *pj = new Particle(1.0, points[1]),
    *pk = new Particle(1.0, points[2]);
  Triangle triangle(pi, pj, pk, &box);

  Vec3 result;
  if (triangle.trilaterate(radius, result)) {

    if (expected_result_pair.first) {

      if (result == expected_result_pair.second) {
        this->pass();
      }
      else {
        this->fail(Formatter() 
            << "Triangle " << pi->get_position() << "--" << pj->get_position() << "--" << pk->get_position() 
            << " should have a point of trilateration at " << expected_result_pair.second
            << ", for a settling particle of radius " << radius << ", but doesn't. Got: " << result << ", instead.");
      }
    }
    else {

      this->fail(Formatter() 
          << "Triangle " << pi->get_position() << "--" << pj->get_position() << "--" << pk->get_position() 
          << " should not have a point of trilateration for a settling particle of radius " << radius << ", but does (" << result << ").");

    }

  }
  else {

    if (expected_result_pair.first) {
      this->fail(Formatter() 
          << "Triangle " << pi->get_position() << "--" << pj->get_position() << "--" << pk->get_position() 
          << " should have a point of trilateration for a settling particle of radius " << radius << ", but doesn't.");
    }
    else {
      this->pass();
    }

  }
}

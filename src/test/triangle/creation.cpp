#include "triangle.hpp"


TriangleCreationTest::TriangleCreationTest(bool is_quiet)
  : TestRunner("Triangle test (creation)", is_quiet)
{
  this->input_data = {
    std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 1.0, 0.0})}),
    std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0}), Vec3({0.0, 9.0, 0.0})}),
    std::array<Vec3, 3>({Vec3({0.0, 0.0, 0.0}), Vec3({1.0, 0.0, 0.0}), Vec3({2.0, 0.0, 0.0})})
  };
  this->expected_results = {
    {"Vertex", "Vertex", "Vertex", "Line", "Line", "Line", "Triangle"},
    {"Vertex", "Vertex", "Vertex", "Line", "Line", "Line", "Triangle"},
    {"Vertex", "Vertex", "Vertex", "Line", "Line", "Line"}
  };
}

std::string repr_strvec(std::vector<std::string> strvec)
{
  std::stringstream ss;
  ss << "[" << strvec[0];
  for (size_t i = 1; i < strvec.size(); i++) {
    ss << "," << strvec[i];
  }
  ss << "]";
  return ss.str();
}

void TriangleCreationTest::run(std::array<Vec3, 3> points, std::vector<std::string> expected_result)
{
  XYPeriodicBox box(10.0, this->is_quiet?-10:0);
  Particle *pi = new Particle(1.0, points[0]),
    *pj = new Particle(1.0, points[1]),
    *pk = new Particle(1.0, points[2]);

  box.add_particle(pi);
  box.add_particle(pj);
  box.add_particle(pk);

  std::list<ParticleArrangement *> arrangements = box.get_arrangements();
  arrangements.sort(ArrangementByNameComparator());

  std::vector<std::string> result;
  for (auto arr : arrangements) {
    result.push_back(arr->get_type());
  }

  if (result == expected_result) {
    this->pass();
  }
  else {
    this->fail(Formatter() 
        << "Unexpected arrangements. Expected "
        << repr_strvec(expected_result) << ", but got "
        << repr_strvec(result) << ".");
  }
}

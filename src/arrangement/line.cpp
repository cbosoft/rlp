#include "line.hpp"


Line::Line(Particle *a, Particle *b, XYPeriodicBox *box)
{
  this->particles[0] = a;
  this->particles[1] = b;
  this->box = box;
  this->dist = this->box->get_effective_separation(a->get_position(), b->get_position()).magnitude();
  this->supportable_size = this->dist - a->get_radius() - b->get_radius();
}


bool Line::check_interacts_with(const Particle *p)
{
  // could the line support the particle?
  if (this->supportable_size > p->get_diameter()) {
    //std::cerr << "no because wide" << std::endl;
    return false;
  }

  // is the particle close enough to either line point?
  int close_enough = 0;
  Vec3 ppos3 = p->get_position();
  Vec2 ppos = ppos3.restrict<2>();
  for (size_t i = 0; i < this->particles.size(); i++) {
    Vec2 vpos = this->particles[i]->get_position().restrict<2>();
    double d = this->box->get_effective_separation(ppos, vpos).magnitude();

    // check if particle hits line not just end
    if (d > this->dist) {
      //std::cerr << "no because glancing" << std::endl;
      return false;
    }

    // check particle hits at all
    if (d < this->particles[i]->get_radius() + p->get_radius())
      close_enough ++;

    if (ppos3.get(2) <= this->particles[i]->get_position().get(2))
      return false;

  }

  if (!close_enough) {
    //std::cerr << "no because doesn't touch" << std::endl;
    return false;
  }

  return true;
}


Vec3 Line::get_interaction_result(const Particle *p)
{
  Vec2 S0 = p->get_position().restrict<2>();

start:
  Vec2 R1 = this->particles[0]->get_position().restrict<2>();
  Vec2 R2 = this->particles[1]->get_position().restrict<2>();
  Vec2 dS = this->box->get_effective_separation(S0, R1);
  Vec2 dR = this->box->get_effective_separation(R2, R1);
  double dRmag = dR.magnitude();
  Vec2 dRu = dR / dRmag;
  Vec2 TS = dS.component_along(dR);
  Vec2 NS = dS - TS; // or plus?
  double NSmag = NS.magnitude();

  if (FLOAT_EQ(NSmag, 0.0)) {
    //throw MathError(Formatter() << "Divide by zero encountered in Line::get_intersection_result!");

    // Math error, jiggle the start point and try again
    S0 = S0 + vec2_urand(-1e-10, 1e-10);
    goto start;
  }

  Vec2 NSu = NS.unit();
  double a = dRmag;
  double b = this->particles[0]->get_radius() + p->get_radius();
  double c = this->particles[1]->get_radius() + p->get_radius();
  double costheta = (a*a + b*b - c*c) / (2*a*b);

  if ((costheta > 1.0) or (costheta < -1.0)) {
    throw MathError(Formatter() << "Invalid costheta value encountered in Line::get_intersection_result! (" << costheta << ").");
  }

  double theta = std::acos(costheta);

  double t = b*costheta;
  Vec2 C = R1 + (dRu*t);

  double l = b*std::sin(theta);
  Vec2 S = C + (NSu*l);

  double h1 = this->particles[0]->get_position().get(2);
  double h2 = this->particles[2]->get_position().get(2);

  return S.promote<3>( (h1>h2) ? h2 : h1 );
}

Vec3 Line::get_frictional_interaction_result(const Particle *p)
{
  double time = 0.0, result_time = 1e9;
  Vec3 position, result_position;
 
  bool interacts = false;
  for (auto particle : this->particles) { 
    bool res = this->get_two_particle_frictional_interaction(p, particle, time, position);

    interacts |= res;

    if (time < result_time)
      result_position = position;

  }

  if (not interacts)
    throw IntersectionError("Intersection failed (line)");

  return result_position;
}


double Line::get_sort_distance(const Particle *p)
{
  double mindist = this->box->get_effective_separation(p->get_position(), this->particles[0]->get_position()).magnitude();
  double maxdist = mindist;
  for (size_t i = 1; i < this->particles.size(); i++) {
    // no PBC (abs distance only)
    double dist = this->box->get_effective_separation(p->get_position(), this->particles[i]->get_position()).magnitude();
    if (dist < mindist)
      mindist = dist;
    if (dist > maxdist)
      maxdist = dist;
  }
  return mindist*100.0 + maxdist + 1.0;
}


double Line::get_max_distance(const Particle *p)
{
  (void)p;
  double total = 0.0;
  for (size_t i = 0; i < this->particles.size(); i++) {
    total += this->box->get_effective_separation(this->particles[i]->get_position().restrict<2>(), p->get_position().restrict<2>()).magnitude();
  }
  return total/double(this->particles.size());
}

double Line::get_min_distance(const Particle *p)
{
  // distance of falling particle to particle closest in xy within arrangement

  Vec3 position = p->get_position();
  Vec2 position_xy = position.restrict<2>();
  Vec3 closest = this->particles[0]->get_position();
  double min_dist_xy = this->box->get_effective_separation(closest.restrict<2>(), position_xy).magnitude();
  for (size_t i = 1; i < this->particles.size(); i++) {
    double dist_xy = this->box->get_effective_separation(position_xy, this->particles[i]->get_position().restrict<2>()).magnitude();
    if (dist_xy < min_dist_xy) {
      closest = this->particles[i]->get_position();
      min_dist_xy = dist_xy;
    }
  }

  return this->box->get_effective_separation(closest, position).magnitude();
}


double Line::get_z_position()
{
  double maxh = this->box->get_effective_position(this->particles[0]->get_position()).get(2);
  for (size_t i = 1; i < this->particles.size(); i++) {
    // no PBC (abs distance only)
    double h = this->box->get_effective_position(this->particles[i]->get_position()).get(2);
    if (h > maxh)
      maxh = h;
  }
  return maxh;
}


std::vector<Vec3> Line::get_extents()
{
  std::vector<Vec3> rv;

  for (auto particle : this->particles) {
    rv.push_back(particle->get_position());
  }

  return rv;
}

std::string Line::repr()
{
  std::stringstream ss;
  ss << this->get_type() << "("
    << this->particles[0]->get_position() << "-"
    << this->particles[1]->get_position() << ")";
  return ss.str();
}

Vec3 Line::get_centre() const
{
  Vec3 total;
  for (size_t i = 0; i < this->particles.size(); i++) {
    total = total + this->particles[i]->get_position();
  }
  total = total / double(this->particles.size());
  return total;
}

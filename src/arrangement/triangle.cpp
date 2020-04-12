#include "triangle.hpp"


Triangle::Triangle(Particle *a, Particle *b, Particle *c, XYPeriodicBox *box)
{
  this->particles[0] = a;
  this->particles[1] = b;
  this->particles[2] = c;
  this->box = box;

  // TODO precompute distances
  for (size_t i = 0; i < this->particles.size(); i++) {
    Particle *pi = this->particles[i];
    Vec2 ipos = pi->get_position().restrict<2>();
    
    int j = (i+1)%3;
    Particle *pj = this->particles[j];
    Vec2 jpos = pj->get_position().restrict<2>();

    int k = (i+2)%3;
    Particle *pk = this->particles[k];
    Vec2 kpos = pk->get_position().restrict<2>();

    Vec2 r_ij = this->box->get_effective_separation(jpos, ipos);
    Vec2 r_ik = this->box->get_effective_separation(kpos, ipos);

    this->edges[i] = r_ij;
    this->edge_distances[i] = this->edges[i].magnitude();
    this->internal_angles[i] = r_ij.angle_between(r_ik);
  }
}

bool Triangle::check_interacts_with(const Particle *p)
{

  int close_enough = 0;
  Vec3 ppos3 = p->get_position();
  Vec2 ppos = ppos3.restrict<2>();
  for (size_t i = 0; i < this->particles.size(); i++) {
    Particle *pi = this->particles[i];

    if (ppos3.get(2) < pi->get_position().get(2)) {
      return false;
    }

    Vec2 ipos = pi->get_position().restrict<2>();
    Vec2 r_ip = this->box->get_effective_separation(ppos, ipos);

    double d = r_ip.magnitude();
    if (d < this->particles[i]->get_radius() + p->get_radius())
      close_enough ++;

    // if (d > edge_distances[i]) {
    //   return false;
    // }
    
    double theta = r_ip.angle_between(this->edges[i]);
    if (theta > this->internal_angles[i])
      return false;


  }

  if (!close_enough) {
    // particle doesn't touch any of the corners
    return false;
  }

  Vec3 temp;
  return this->trilaterate(p->get_radius(), temp);
}

Vec3 Triangle::get_interaction_result(const Particle *p)
{
  Vec3 new_position;
  if (this->trilaterate(p->get_radius(), new_position)) {
    return new_position;
  }

  throw SettleError(Formatter() << "Particle (r=" << p->get_radius() << ") was asked to settle in triangle that cannot accomodate it (trilateration failed).");
}

Vec3 Triangle::get_frictional_interaction_result(const Particle *p)
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
    throw IntersectionError("Intersection failed (triangle)");

  return result_position;
}

double Triangle::get_sort_distance(const Particle *p)
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
  return mindist*100.0 + maxdist + 0.0;
}

// double Triangle::get_max_distance(const Particle *p)
// {
//   int closest_index = this->get_closest_particle_index(p);
// 
//   double edge1 = this->edge_distances[closest_index];
//   double edge2 = this->edge_distances[(closest_index+2)%3];
// 
//   return std::max(edge1, edge2);
// }
double Triangle::get_max_distance(const Particle *p)
{
  (void)p;
  double total = 0.0;
  for (size_t i = 0; i < this->particles.size(); i++) {
    total += this->box->get_effective_separation(this->particles[i]->get_position().restrict<2>(), p->get_position().restrict<2>()).magnitude();
  }
  return total/double(this->particles.size());
}

double Triangle::get_min_distance(const Particle *p)
{
  // distance of falling particle to particle closest in xy within arrangement

  Vec3 closest = this->particles[this->get_closest_particle_index(p)]->get_position();
  Vec3 position = p->get_position();
  return this->box->get_effective_separation(closest, position).magnitude();
}


bool Triangle::trilaterate(double radius, Vec3 &rv)
{
  // https://stackoverflow.com/questions/1406375/finding-intersection-points-between-3-spheres/18654302#18654302
  Vec3 r_ji = this->box->get_effective_separation(this->particles[1]->get_position(), this->particles[0]->get_position());
  double d = r_ji.magnitude();
  Vec3 e_x = r_ji / d;
  Vec3 r_ki = this->box->get_effective_separation(this->particles[2]->get_position(), this->particles[0]->get_position());
  double i = e_x.dot(r_ki);
  Vec3 e_y = (r_ki - e_x*i).unit();
  Vec3 e_z = e_y.cross(e_x);
  double j = e_y.dot(r_ki);
  double r1 = this->particles[0]->get_radius()+radius, r2 = this->particles[1]->get_radius()+radius, r3 = this->particles[2]->get_radius()+radius;
  double x = (r1*r1 - r2*r2 + d*d) / (2*d);
  double y = (r1*r1 - r3*r3 -2*i*x + i*i + j*j) / (2*j);
  double z2 = r1*r1 - x*x - y*y;
  if (z2 < 0.0)
    return false;

  double z = std::pow(z2, 0.5);
  Vec3 effpos = this->box->get_effective_position(this->particles[0]->get_position());
  Vec3 solution_a = effpos + e_x*x + e_y*y + e_z*z;
  Vec3 solution_b = effpos + e_x*x + e_y*y - e_z*z;

  if (solution_a.get(2) > solution_b.get(2)) {
    rv = solution_a;
    return true;
  }
  rv = solution_b;
  return true;
}


double Triangle::get_z_position()
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


std::vector<Vec3> Triangle::get_extents()
{
  std::vector<Vec3> rv;

  for (auto particle : this->particles) {
    rv.push_back(particle->get_position());
  }

  return rv;
}

bool Triangle::covers(ParticleArrangement *arr)
{
  std::vector<Vec3> points = arr->get_extents();

  for (auto point : points) {
    point.set(2, 1e9);
    Particle *p = new Particle(1.0, point);
    if (this->check_interacts_with(p))
      return true;
    delete p;
  }

  return false;
}

std::string Triangle::repr()
{
  std::stringstream ss;
  ss << this->get_type() << "("
    << this->particles[0]->get_position() << "-"
    << this->particles[1]->get_position() << "-"
    << this->particles[2]->get_position() << ")";
  return ss.str();
}

int Triangle::get_closest_particle_index(const Particle *p)
{
  Vec3 position = p->get_position();
  Vec2 position_xy = position.restrict<2>();
  Vec3 closest = this->particles[0]->get_position();
  int index = 0;
  double min_dist_xy = this->box->get_effective_separation(closest.restrict<2>(), position_xy).magnitude();
  for (size_t i = 1; i < this->particles.size(); i++) {
    double dist_xy = this->box->get_effective_separation(position_xy, this->particles[i]->get_position().restrict<2>()).magnitude();
    if (dist_xy < min_dist_xy) {
      closest = this->particles[i]->get_position();
      min_dist_xy = dist_xy;
      index = i;
    }
  }

  return index;
}

Vec3 Triangle::get_centre() const
{
  Vec3 total;
  for (size_t i = 0; i < this->particles.size(); i++) {
    total = total + this->particles[i]->get_position();
  }
  total = total / double(this->particles.size());
  return total;
}

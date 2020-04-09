#include <chrono>

#include "config.hpp"
#include "../util/random.hpp"
#include "../util/vec.hpp"
#include "../util/exception.hpp"

#define CLOCK std::chrono::steady_clock

#define INC_ERROR_COUNT \
  if ( (error_tolerance < 0) or (errors < error_tolerance) ) {\
    errors ++;\
  }\
  else {\
    throw e;\
  }


void ConfigGenerator::generate_particles(int n, int error_tolerance)
{

  this->box.reserve(n);

  int errors = 0;
  double generation_duration = -1;
  int i = 0;
  auto last_print = CLOCK::now();
  double time_since_print_duration = 100;
  while ((i = this->box.get_number_particles()) < n) {

    auto before = CLOCK::now();
    time_since_print_duration = static_cast<double>((before - last_print).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
    if (time_since_print_duration > 1.0) {

      last_print = CLOCK::now();
      std::cerr << BG_BLUE << "(" << this->box.get_number_particles() << "/" << n << ") (" << generation_duration << ") (" << errors << "/" << error_tolerance << ")" RESET " ";

      if (this->verbosity < 1) {
        std::cerr << std::endl;
      }
    }

    before = CLOCK::now();

    try {
      this->generate_particle();
    }
    catch (const SettleError& e) {
      // do nothing
    }
    catch (const IntersectionError& e) {
      INC_ERROR_COUNT;
    }
    catch (const MathError& e) {
      INC_ERROR_COUNT;
    }
    catch (const RecursionError& e) {
      INC_ERROR_COUNT;
    }
    catch (const Exception& e) {
      INC_ERROR_COUNT;
    }

    auto after = CLOCK::now();
    generation_duration = static_cast<double>((after - before).count()) * CLOCK::duration::period::num / CLOCK::duration::period::den;
  }

  std::cerr << BG_BLUE << this->box.get_volume_fraction() << RESET << std::endl;
}

void ConfigGenerator::generate_particle()
{
  Vec3 position = vec3_urand(0.0, this->box.get_L());
  double d = this->sieve->get_size();
  position.set(2, this->box.get_L());
  Particle *p = new Particle(d, position);
  this->box.add_particle(p);
}

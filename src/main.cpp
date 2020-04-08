#include "config.hpp"
#include "test/run_tests.hpp"
#include "exception.hpp"
#include "random.hpp"
#include "sieve.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{

  struct {
    int number;
    double length;
    const char *output_path;
    const char *disperse;
    // double mean;
    // double std;
    int seed;
    int verbosity;
    int error_tolerance;
    bool run_tests;
    bool output_on_error;
    bool particles_are_seed;
    double friction_thresh;
  } args = {
    .number = 100,
    .length = 10.0,
    .output_path = "out.csv",
    .disperse = "mono",
    // .mean = 1.0,
    // .std = 0.0,
    .seed = 1,
    .verbosity = 1,
    .error_tolerance = 0,
    .run_tests = true,
    .output_on_error = true,
    .particles_are_seed = false,
    .friction_thresh = 2.0
  };

  argc--; argv++;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-n", "--number")) {
      args.number = std::atoi(argv[++i]);
    }
    else if (EITHER("-l", "--length")) {
      args.length = std::atof(argv[++i]);
    }
    else if (EITHER("-o", "--output-path")) {
      args.output_path = argv[++i];
    }
    else if (EITHER("-v", "--verbose")) {
      args.verbosity ++;
    }
    else if (EITHER("-q", "--quiet")) {
      args.verbosity --;
    }
    else if (strcmp(argv[i], "--disperse") == 0) {
      args.disperse = argv[++i];
    }
    else if (EITHER("-f", "--friction-thresh")) {
      args.friction_thresh = std::atof(argv[++i]);
    }
    // else if (EITHER("-m", "--mean")) {
    //   args.mean = atof(argv[++i]);
    // }
    // else if (EITHER("-s", "--std")) {
    //   args.std = atof(argv[++i]);
    // }
    else if (strcmp(argv[i], "--seed") == 0) {
      args.seed = atof(argv[++i]);
    }
    else if (strcmp("--error-tolerance", argv[i]) == 0) {
      args.error_tolerance = atoi(argv[++i]);
    }
    else if (strcmp("--infinitely-tolerate-errors", argv[i]) == 0) {
      args.error_tolerance = -1;
    }
    else if (strcmp("--particles-are-seed", argv[i]) == 0) {
      args.particles_are_seed = true;
    }
    else if (strcmp("--dont-run-tests", argv[i]) == 0) {
      args.run_tests = false;
    }
    else if (strcmp("--dont-output-on-error", argv[i]) == 0) {
      args.output_on_error = false;
    }
    else {
      throw ArgumentError(Formatter() << "Unrecognised argument \"" << argv[i] << "\".");
    }
  }

  if (args.run_tests)
    run_tests();

  ConfigGenerator cg = ConfigGenerator(args.length, args.verbosity, args.particles_are_seed, args.friction_thresh);

  if (strcmp(args.disperse, "mono") == 0) {
    cg.set_sieve(new MonoSieve());
  }
  else if (strcmp(args.disperse, "bi") == 0) {
    cg.set_sieve(new BiSieve());
  }
  else if (strcmp(args.disperse, "altbi") == 0) {
    cg.set_sieve(new AlternatingBiSieve());
  }
  else {
    throw ArgumentError(Formatter() << "Unrecognised dispersion type \"" << args.disperse << "\".");
  }


  seed(args.seed);

  try {
    cg.generate_particles(args.number, args.error_tolerance);
  }
  catch (const Exception &e) {
    if (args.output_on_error)
      cg.output_configuration(args.output_path);
    throw e;
  }

  cg.output_configuration(args.output_path);
}

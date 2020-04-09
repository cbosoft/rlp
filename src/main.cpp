#include "config.hpp"
#include "test/run_tests.hpp"
#include "exception.hpp"
#include "random.hpp"
#include "sieve.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

void show_help_and_exit()
{
  std::cerr 
    << "\n"
    << "  " BOLD "RLP:" RESET " Random Loose Packing\n"
    << "\n"
    << "  " BOLD "Description" RESET "\n"
    << "    RLP forms packings of particles at the loose packing limit; where the\n"
    << "    particles are only just able to support themselves. Different packings\n"
    << "    can be made using different settings. This is intended to be used to\n"
    << "    investigate particle configurations in suspension rheology. Config is\n"
    << "    generated by positioning particles one by one at random, and settling\n"
    << "    them into a box.\n"
    << "\n"
    << "  " BOLD "Options" RESET "\n"
    << "    --number <val>  Number of particles to settle in the box. More particles\n"
    << "                    takes more time, but increases the accuracy of the result.\n"
    << "                    A full box (n = (length/diameter)^3) is recommended min.\n"
    << "                    Default: 100.\n"
    << "\n"
    << "    --length <val>  Length of side of cubic box. Default: 10.0.\n"
    << "\n"
    << "    --output-path <val>  Path to output csv containing the particle\n"
    << "                    configuration. Default: 'out.csv'\n"
    << "\n"
    << "    --disperse <val>  Particles can be created with different sizes. The config\n"
    << "                    generator chooses the size according to a distribution. Valid\n"
    << "                    values are 'mono', 'bi', 'altbi'. Default: 'mono'.\n"
    << "\n"
    << "    --seed <val>    Seed the random number generator with an integer. Default: 1.\n"
    << "\n"
    << "    --verbose       Make the sim more verbose every time this flag appears.\n"
    << "\n"
    << "    --quiet         Make the sim less verbose every time this flag appears.\n"
    << "\n"
    << "    --error-tolerance <val>  Set the number of allowable errors to <v>. This\n"
    << "                    will make the config generator ignore a number of errors.\n"
    << "                    Set to 0 to not ignore errors, set to -1 to ignore all\n"
    << "                    errors. Default: 0.\n"
    << "\n"
    << "    --infinitely-tolerate-errors <val>  Same as '--error-tolerance -1'\n"
    << "\n"
    << "    --friction-thresh <val>  Sets the size threshold for deciding if a particle\n"
    << "                    interacts frictionally. Large particles will act with friction\n"
    << "                    before small particles; a particle with a diameter larger than\n"
    << "                    the threshold will interact frictionally.\n"
    << "\n"
    << "    --particles-are-seed  Sets particles to seed: after a particle is added ('seed'\n"
    << "                    stage), more particles are generated which will surely interact\n"
    << "                    with existing particles ('growth' stage). This will create a\n"
    << "                    configuration more quickly, but more densely. This is probably\n"
    << "                    not RLP, but it is close.\n"
    << "\n"
    << "    --dont-output-on-error  Normally, when an error is encountered (and not\n"
    << "                    tolerated), the sim will write out the particle configuration\n"
    << "                    gathered so far. Setting this flag will stop that from\n"
    << "                    happening.\n"
    << "\n"
    << "    --dont-run-tests  As dangerous as it sounds; setting this flag will stop\n"
    << "                    tests from being run before the config generator starts.\n"
    << "\n"
    << "    --help          Show this information and exit.\n"
    ;

  exit(0);
}

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
    else if (strcmp("--help", argv[i]) == 0) {
      show_help_and_exit();
    }
    else {
      throw ArgumentError(Formatter() << "Unrecognised argument \"" << argv[i] << "\".");
    }
  }

  if (args.run_tests)
    run_tests();

  ConfigGenerator cg = ConfigGenerator(args.length, args.verbosity, args.particles_are_seed, args.friction_thresh, args.output_path);

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
      cg.output_configuration();
    throw e;
  }

  cg.output_configuration();
}

#include "config.hpp"
#include "test/run_tests.hpp"
#include "exception.hpp"
#include "random.hpp"
#include "sieve.hpp"
#include "version.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

void show_help_and_exit()
{
  std::cout 
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
    << "                    Value can also be 'recommended' to get a full box according\n"
    << "                    to: n = 1.5*(length/mean_diameter)^3. Default: 'recommended'.\n"
    << "\n"
    << "    --length <val>  Length of side of cubic box. Default: 10.0.\n"
    << "\n"
    << "    --output-path <val>  Path to output csv containing the particle\n"
    << "                    configuration. Default: 'out.csv'\n"
    << "\n"
    << "    --seive <val>   Particles can be created with different sizes. The config\n"
    << "                    generator chooses the size according to a distribution,\n"
    << "                    defined by a 'sieve'. Valid values are 'mono', 'bi', \n"
    << "                    and 'altbi'. See 'Sieve Options' below. Default: 'mono'.\n"
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
    << "\n"
    << "  " BOLD "Sieve Options" RESET "\n"
    << "\n"
    << "    " BOLD "Monodisperse" RESET"\n"
    << "      Mono disperse sieve has no options; all particles are given the same size.\n"
    << "\n"
    << "    " BOLD "Bidisperse" RESET"\n"
    << "      Bidisperse sieve produces particles of one of two sizes. There is always a\n"
    << "      particle of size 1.0, as this is the scale. The smaller size is speficied\n"
    << "      by a ratio of smallest diameter to largest. (Although, as the large\n"
    << "      diameter is 1, the small diameter is the same as the ratio.) Finally, size\n"
    << "      is decided based on a probability of being large or small.\n"
    << "\n"
    << "      --bi-ratio    Ratio of small diameter to small diameter or, equivalently,\n"
    << "                    the size of the small diameter in units of large diamter."
    << "\n"
    << "      --bi-probability    Probability of a particle having the larger diameter.\n"
    << "\n"
    << "    " BOLD "Bidisperse (alternating)" RESET"\n"
    << "      Alternating bidisperse chooses a particle size based on a ratio as for\n"
    << "      plain bidisperse, but instead of a probability it simply alternates\n"
    << "      between large and small. See '--bi-ratio' above.\n"
    ;

  exit(0);
}

int main(int argc, const char **argv)
{

  struct {
    // number and density
    int number;
    double length;

    // dispersity
    const char *sieve_type;
    double bi_ratio;
    double bi_probability;
    //double normal_std;

    // run
    const char *output_path;
    int seed;
    int verbosity;
    int error_tolerance;
    bool run_tests;
    bool output_on_error;
    bool particles_are_seed;
    double friction_thresh;
  } args = {
    .number = -1,
    .length = 10.0,
    
    .sieve_type = "mono",
    .bi_ratio = 0.5,
    .bi_probability = 0.5,

    .output_path = "out.csv",
    .seed = 1,
    .verbosity = 0,
    .error_tolerance = 0,
    .run_tests = true,
    .output_on_error = true,
    .particles_are_seed = false,
    .friction_thresh = 2.0
  };

  argc--; argv++;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-n", "--number")) {

      if (strcmp(argv[++i], "recommended") == 0)
        args.number = -1;
      else
        args.number = std::atoi(argv[i]);

    }
    else if (EITHER("-l", "--length")) {
      args.length = std::atof(argv[++i]);
    }
    else if (EITHER("-o", "--output-path")) {
      args.output_path = argv[++i];
    }
    else if (strcmp(argv[i], "--bi-ratio") == 0) {
      args.bi_ratio = std::atof(argv[++i]);
    }
    else if (strcmp(argv[i], "--bi-probability") == 0) {
      args.bi_probability = std::atof(argv[++i]);
    }
    else if (EITHER("-v", "--verbose")) {
      args.verbosity ++;
    }
    else if (EITHER("-q", "--quiet")) {
      args.verbosity --;
    }
    else if (strcmp(argv[i], "--sieve") == 0) {
      args.sieve_type = argv[++i];
    }
    else if (EITHER("-f", "--friction-thresh")) {
      args.friction_thresh = std::atof(argv[++i]);
    }
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
    run_tests(args.verbosity < 1);

  ConfigGenerator cg = ConfigGenerator(args.length, args.verbosity, args.particles_are_seed, args.friction_thresh, args.output_path);

  if (strcmp(args.sieve_type, "mono") == 0) {
    cg.set_sieve(new MonoSieve());
  }
  else if (strcmp(args.sieve_type, "bi") == 0) {
    cg.set_sieve(new BiSieve(args.bi_ratio, args.bi_probability));
  }
  else if (strcmp(args.sieve_type, "altbi") == 0) {
    cg.set_sieve(new AlternatingBiSieve(args.bi_ratio));
  }
  else {
    throw ArgumentError(Formatter() << "Unrecognised sieve type \"" << args.sieve_type << "\".");
  }
  double mean_diameter = cg.get_sieve()->get_mean();

  if (args.number < 0) {
    double rho1d = args.length / mean_diameter;
    args.number = int(1.5*rho1d*rho1d*rho1d);
  }

  if (args.verbosity >= 0) {
    std::cerr 
      << BOLD "RLP" RESET ": Random Loose Packing " << VERSION <<"\n"
      << "Generating configuration of " << args.number << " particles in a box of side " << args.length << ".\n"
      ;

    if (strcmp(args.sieve_type, "mono") == 0) {
      std::cerr << "Particles are monodisperse with diameter of " << mean_diameter << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "bi") == 0) {
      std::cerr << "Particles are bi-disperse. Diameter has " 
        << (1.0 - args.bi_probability)*100.0 << "% chance of being " << args.bi_ratio << ". Mean diameter is " << mean_diameter << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "altbi") == 0) {
      std::cerr << "Particles are bi-disperse (alternating). Diameter will alternate between 1.0 and " << args.bi_ratio
        << ". Mean diameter is " << mean_diameter << "." << std::endl;
    }

    std::cerr << "Config file will be written to " << args.output_path << std::endl;
    if (args.error_tolerance > 0) {
      std::cerr << args.error_tolerance << " non-trivial errors will be tolerated." << std::endl;
    }
    else if (args.error_tolerance < 0) {
      std::cerr << "All non-trivial errors will be tolerated." << std::endl;
    }
    else {
      std::cerr << "No non-trivial errors will be tolerated." << std::endl;
    }

  }


  seed(args.seed);

  try {
    cg.generate_particles(args.number, args.error_tolerance);
  }
  catch (const Exception &e) {
    if (args.output_on_error)
      cg.output_configuration(true);
    throw e;
  }

  cg.output_configuration();
}

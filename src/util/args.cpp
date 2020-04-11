#include <cstring>

#include "exception.hpp"
#include "args.hpp"
#include "help.hpp"
#include "../version.hpp"

#define ARGEQ(S) (strcmp(argv[i], S) == 0)

CommandLineArguments get_default_args()
{
  CommandLineArguments args = {
    .number = -1,
    .length = 10.0,
    .mean_diameter = 1.0,
    
    .sieve_type = "mono",
    .bi_ratio = 0.5,
    .bi_probability = 0.5,
    .normal_std = 0.1,
    .uniform_min = 0.5,

    .output_path = "out.csv",
    .seed = 1,
    .verbosity = 0,
    .error_tolerance = -2,
    .run_tests = true,
    .output_on_error = true,
    .particles_are_seed = false,
    .friction_thresh = 2.0
  };
  return args;
}


CommandLineArguments parse_args(int argc, const char **argv)
{

  CommandLineArguments args = get_default_args();
  argc--; argv++;
  for (int i = 0; i < argc; i++) {
    if (ARGEQ("--number")) {

      i++;
      if (ARGEQ("recommended"))
        args.number = -1;
      else
        args.number = std::atoi(argv[i]);

    }
    else if (ARGEQ("--length")) {
      args.length = std::atof(argv[++i]);
    }
    else if (ARGEQ("--output-path")) {
      args.output_path = argv[++i];
    }
    else if (ARGEQ("--bi-ratio")) {
      args.bi_ratio = std::atof(argv[++i]);
    }
    else if (ARGEQ("--bi-probability")) {
      args.bi_probability = std::atof(argv[++i]);
    }
    else if (ARGEQ("--verbose")) {
      args.verbosity ++;
    }
    else if (ARGEQ("--quiet")) {
      args.verbosity --;
    }
    else if (ARGEQ("--sieve")) {
      args.sieve_type = argv[++i];
    }
    else if (ARGEQ("--friction-thresh")) {
      args.friction_thresh = std::atof(argv[++i]);
    }
    else if (ARGEQ("--normal-std")) {
      args.normal_std = atof(argv[++i]);
    }
    else if (ARGEQ("--uniform-min")) {
      args.uniform_min = atof(argv[++i]);
    }
    else if (ARGEQ("--seed")) {
      args.seed = atof(argv[++i]);
    }
    else if (ARGEQ("--error-tolerance")) {

      i++;
      if (ARGEQ("all"))
        args.error_tolerance = -1;
      else if (ARGEQ("most"))
        args.error_tolerance = -2;
      else
        args.error_tolerance = atoi(argv[i]);

    }
    else if (ARGEQ("--infinitely-tolerate-errors")) {
      args.error_tolerance = -1;
    }
    else if (ARGEQ("--particles-are-seed")) {
      args.particles_are_seed = true;
    }
    else if (ARGEQ("--dont-run-tests")) {
      args.run_tests = false;
    }
    else if (ARGEQ("--dont-output-on-error")) {
      args.output_on_error = false;
    }
    else if (ARGEQ("--progress-bar")) {
      args.verbosity = 0;
    }
    else if (ARGEQ("--progress-json")) {
      args.verbosity = -1;
    }
    else if (ARGEQ("--help")) {
      show_help_and_exit();
    }
    else {
      throw ArgumentError(Formatter() << "Unrecognised argument \"" << argv[i] << "\".");
    }
  }

  return args;
}



void show_run_info(CommandLineArguments &args)
{
  if (args.verbosity >= 0) {
    std::cerr 
      << BOLD "RLP" RESET ": Random Loose Packing " << VERSION <<"\n"
      << "Generating configuration of " << args.number << " particles in a box of side " << args.length << ".\n"
      ;

    if (strcmp(args.sieve_type, "mono") == 0) {
      std::cerr << "Particles are monodisperse with diameter of " << args.mean_diameter << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "bi") == 0) {
      std::cerr << "Particles are bi-disperse. Diameter has " 
        << (1.0 - args.bi_probability)*100.0 << "% chance of being " << args.bi_ratio << ". Mean diameter is " << args.mean_diameter << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "altbi") == 0) {
      std::cerr << "Particles are bi-disperse (alternating). Diameter will alternate between 1.0 and " << args.bi_ratio
        << ". Mean diameter is " << args.mean_diameter << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "normal") == 0) {
      std::cerr 
        << "Particles are polydisperse (normally distributed). Diameter will be chosen from\n"
        << "a normal distribution of mean " << args.mean_diameter << " and standard deviation " << args.normal_std << "." << std::endl;
    }
    else if (strcmp(args.sieve_type, "uniform") == 0) {
      std::cerr 
        << "Particles are polydisperse (uniformly distributed). Diameter will be chosen from\n"
        << "a uniform distribution between " << args.uniform_min << " and 1.0. Mean diameter: " << args.mean_diameter << "." << std::endl;
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
}

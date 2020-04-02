#include "config.hpp"
#include "test/run_tests.hpp"
#include "exception.hpp"

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))

int main(int argc, const char **argv)
{

  struct {
    int number;
    double length;
    const char *output_path;
    // double mean;
    // double std;
    bool run_tests;
  } args = {
    .number = 100,
    .length = 10.0,
    .output_path = "out.csv",
    // .mean = 1.0,
    // .std = 0.0,
    .run_tests = true
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
      args.output_path = argv[i];
    }
    // else if (EITHER("-m", "--mean")) {
    //   args.mean = atof(argv[++i]);
    // }
    // else if (EITHER("-s", "--std")) {
    //   args.std = atof(argv[++i]);
    // }
    else if (strcmp("--dont-run-tests", argv[i]) == 0) {
      args.run_tests = false;
    }
    else {
      throw ArgumentError(Formatter() << "Unrecognised argument \"" << argv[i] << "\".");
    }
  }

  if (args.run_tests)
    run_tests();

  ConfigGenerator cg = ConfigGenerator(args.length);
  cg.generate_particles(args.number);
  cg.output_configuration(args.output_path);
}

#pragma once

typedef struct command_line_arguments {
  // number and density
  int number;
  double length;
  double mean_diameter; // not set; calculated.

  // dispersity
  const char *sieve_type;
  double bi_ratio;
  double bi_probability;
  double normal_std;
  double uniform_min;

  // run
  const char *output_path;
  int seed;
  int verbosity;
  int error_tolerance;
  bool run_tests;
  bool output_on_error;
  bool particles_are_seed;
  double friction_thresh;
} CommandLineArguments;

CommandLineArguments parse_args(int argc, const char **argv);
void show_run_info(CommandLineArguments &args);

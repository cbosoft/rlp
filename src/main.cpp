#include "config/config.hpp"
#include "test/run_tests.hpp"
#include "util/exception.hpp"
#include "util/random.hpp"
#include "sieve/sieve.hpp"
#include "version.hpp"
#include "util/args.hpp"
#include "box/xyperiodic/xyperiodicbox.hpp"


int main(int argc, const char **argv)
{

  CommandLineArguments args = parse_args(argc, argv);

  if (args.run_tests)
    run_tests(args.verbosity < 1);

  ConfigGenerator cg = ConfigGenerator(args.verbosity, args.output_path);
  // TODO read boxtype from args
  GenericBox *box = new XYPeriodicBox(args.length, args.verbosity, args.friction_thresh);
  cg.set_box(box);

  if (strcmp(args.sieve_type, "mono") == 0) {
    cg.set_sieve(new MonoSieve());
  }
  else{ 
  
    if (args.error_tolerance == 0) {
      throw ArgumentError(Formatter() << "Using a polydisperse sieve with no error tolerance will very likely end early due to error.");
    }
  
    if (strcmp(args.sieve_type, "bi") == 0) {
      cg.set_sieve(new BiSieve(args.bi_ratio, args.bi_probability));
    }
    else if (strcmp(args.sieve_type, "altbi") == 0) {
      cg.set_sieve(new AlternatingBiSieve(args.bi_ratio));
    }
    else if (strcmp(args.sieve_type, "normal") == 0) {
      cg.set_sieve(new NormalSieve(args.normal_std));
    }
    else if (strcmp(args.sieve_type, "uniform") == 0) {
      cg.set_sieve(new UniformSieve(args.uniform_min));
    }
    else {
      throw ArgumentError(Formatter() << "Unrecognised sieve type \"" << args.sieve_type << "\".");
    }
  }
  args.mean_diameter = cg.get_sieve()->get_mean();

  if (args.number < 0) {
    // TODO: fix this. Number required is under-estimated for polydisperse
    double n_per_l = args.length / args.mean_diameter;
    double n_per_l_p1 = (args.length + 1) / args.mean_diameter;
    args.number = int(n_per_l_p1*n_per_l*n_per_l);
  }

  if (args.error_tolerance == -2) {
    args.error_tolerance = args.number;
  }

  show_run_info(args);

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

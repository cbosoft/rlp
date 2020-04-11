#include <iostream>

#include "help.hpp"
#include "colour.hpp"
#include "../version.hpp"

void show_help_and_exit()
{
  std::cout 
    << "\n"
    << "  " BOLD "RLP:" RESET " Random Loose Packing " VERSION "\n"
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
    << "                    'altbi', 'normal', and 'uniform'. \n"
    << "                    See 'Sieve Options' below. Default: 'mono'.\n"
    << "\n"
    << "    --seed <val>    Seed the random number generator with an integer. Default: 1.\n"
    << "\n"
    << "    --verbose       Make the sim more verbose every time this flag appears. More\n"
    << "                    verbose output means more detail, that may be harder to\n"
    << "                    follow at a glance but will be more useful when tracing an \n"
    << "                    error. This will primarily change the way progress is\n"
    << "                    displayed. See the '--progress-xx' flags below.\n"
    << "\n"
    << "    --quiet         Make the sim less verbose every time this flag appears.\n"
    << "\n"
    << "    --progress-bar  Set the verbosity low such that progress is indicated by\n"
    << "                    simple graphical bars.\n"
    << "\n"
    << "    --progress-json  Set the verbosity at its minimum: use JSON structured output\n"
    << "                    intended to be parsed by an outside program.\n"
    << "\n"
    << "    --error-tolerance <val>  Set the number of allowable errors to <v>. This\n"
    << "                    will make the config generator ignore a number of errors.\n"
    << "                    Set to 0 to not ignore errors, set to -1 to ignore all\n"
    << "                    errors. Can also be set to 'all' to ignore all, or set\n"
    << "                    to 'most' to ignore errors until it becomes ridiculous.\n"
    << "                    Default: most.\n"
    << "\n"
    << "    --infinitely-tolerate-errors <val>  Same as '--error-tolerance all'\n"
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
    << "    " BOLD "Monodisperse" RESET " ('mono')\n"
    << "      Mono disperse sieve has no options; all particles are given the same size.\n"
    << "\n"
    << "    " BOLD "Bidisperse" RESET " ('bi')\n"
    << "      Bidisperse sieve produces particles of one of two sizes. There is always a\n"
    << "      particle of size 1.0, as this is the scale. The smaller size is speficied\n"
    << "      by a ratio of smallest diameter to largest. (Although, as the large\n"
    << "      diameter is 1, the small diameter is the same as the ratio.) Finally, size\n"
    << "      is decided based on a probability of being large or small.\n"
    << "\n"
    << "      --bi-ratio    Ratio of small diameter to small diameter or, equivalently,\n"
    << "                    the size of the small diameter in units of large diamter.\n"
    << "                    Default: 0.5.\n"
    << "\n"
    << "      --bi-probability    Probability of a particle having the larger diameter.\n"
    << "                    Default: 0.5.\n"
    << "\n"
    << "    " BOLD "Bidisperse (alternating)" RESET " ('altbi')\n"
    << "      Alternating bidisperse chooses a particle size based on a ratio as for\n"
    << "      plain bidisperse, but instead of a probability it simply alternates\n"
    << "      between large and small. See '--bi-ratio' above.\n"
    << "\n"
    << "    " BOLD "Polydisperse (normal)" RESET " ('normal')\n"
    << "      This sieve generates sizes according to a normal distribution. The\n"
    << "      standard deviation is used to decide the width of the distribution\n"
    << "      (i.e. the variety of sizes) as well as the mean. Most (99.7%) of values\n"
    << "      are within 6 standard deviations, so the mean is taken to be 1.0 - 3*std,\n"
    << "      to enforce 1.0 as the maximum particle size.\n"
    << "\n"
    << "      --normal-std <val>  Standard deviation in sizes of particles. Default: 0.1\n"
    << "\n"
    << "    " BOLD "Polydisperse (uniform)" RESET " ('uniform')\n"
    << "      A uniform distribution of particle sizes is generated using this sieve.\n"
    << "      This means that there is equal probability of a size being chosen between\n"
    << "      two limits. The upper limit is 1.0, and the lower is user configurable."
    << "\n"
    << "      --uniform-min <val>  Minimum size. Default: 0.5.\n"
    << "\n"
    ;

  exit(0);
}

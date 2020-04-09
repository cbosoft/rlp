# RLP: Really Lame Packer

Rather Lively Peacock forms configurations of red little particles in random
loose packing (minimum concentration for self-supporting structures).

# Status

Randy Looking Porcupines is in almost there. The racontour-like particles are
settling in a manner like Random Loose Packing (i.e. volume fraction approx 0.55
for monodisperse spheres). Rampant Lavish Peas can create configurations of
(mono- and bidisperse) particles approaching Random Loose Packing.

Rakish Lounging Pandas outputs the configuration to .csv, where the first line
contains some meta data (box size, volume fraction, friction fraction) and
subsequent lines are particle diameter and position. A Really Lovely Plotting
script is included in the [scripts](scripts) directory.

Frictional interaction is off by default. It can be turned on for all particles
(set --friction-thresh 0), or only Relatively Large Particles (for a thresh
inbetween particle diameters in polydisperse configurations. This is to mimic
the conditions in suspension where particles are theorised to be separated at
rest by lubrication, but interact frictionally under flow. As flow (stress)
increases, the larger particles will start to overcome the lubrication first,
hence the thresholding method.

# TODO

  - [ ] Add more particle size distribution (normal, uniform, exponential,
    power-law...)
  - [ ] Add tests for frictional interaction
  - [ ] Add tests for volume fraction calculation

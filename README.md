# RLP: Really Lame Packer v0.2

Rather Lively Peacock forms configurations of red little particles in random
loose packing (minimum concentration for self-supporting structures).

# Status

Randy Looking Porcupines is in almost there. The racontour-like particles are
settling to something like Random Loose Packing (i.e. volume fraction approx 0.55
for monodisperse spheres).

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

  - [ ] Add tests for frictional interaction
  - [ ] Detect recursion depth in intersection/settling loop

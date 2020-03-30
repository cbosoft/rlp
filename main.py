import sys

import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib import pyplot as plt

from particle import Particle
from sim import Sim
from test import run_tests

if __name__ == "__main__":

    argv = sys.argv[1:]
    args = {
            '--output':'out.csv',
            '--number':100,
            '--length':10.0,
            '--diameter':1.0,
            '--seed':1,
            '--plot':None,
            '--verbosity': 1
        }
    if argv:
        assert len(argv) % 2 == 0
        specified_args = {k:v for k,v in zip(argv, argv[1:])}
        args = {**args, **specified_args}

    run_tests(True)

    print('generating particles')
    sim = Sim(float(args['--length']), seed=int(args['--seed']), verbose=int(args['--verbosity']))
    for i in range(int(args['--number'])):
        sim.generate_particle(float(args['--diameter']))

    volume_particles = 0.0
    t = 0.0
    for particle in sim.particles:
        volume_particles += (np.pi/6.)*(particle.diameter**3)
        if particle.position[2] > t:
            t = particle.position[2]

    volume_box = sim.L*sim.L*(t+1.0)
    print('volfrac =', volume_particles / volume_box)

    print('outputting')
    with open(args['--output'], 'w') as f:
        for particle in sim.particles:
            x, y, z = particle.position
            f.write(f'{x},{y},{z}\n')


    if args['--plot']:
        print('plotting')

        fig, axes = plt.subplots(nrows=3)
        cmap = cm.get_cmap('viridis', 12)
        for ax in axes:
            ax.set_ylim([-1, sim.L+1])
            ax.set_xlim([-1, sim.L+1])


        particles = list(sim.particles)
        plt.sca(axes[0])
        for i, ax in enumerate(axes):
            particles = list(sorted(particles, key=lambda p:p.position[i]))
            for particle in particles:
                xy = list(particle.position)
                z = xy.pop(i)
                ax.add_patch(plt.Circle(xy, particle.diameter/2.0, ec='black', fc=cmap(z/sim.L)))
        plt.savefig(args['--plot'])


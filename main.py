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

        triangles = sim.triangles

        ax = plt.axes([0, 0, 1, 1], projection='3d')

        u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
        X = np.cos(u)*np.sin(v)*0.5
        Y = np.sin(u)*np.sin(v)*0.5
        Z = np.cos(v)*0.5

        cmap = cm.get_cmap('viridis', 12)
        for particle in sim.particles:
            x = np.add(X, particle.position[0])
            y = np.add(Y, particle.position[1])
            z = np.add(Z, particle.position[2])
            nz = np.divide(particle.position[2], sim.L)
            ax.plot_surface(x, y, z, color=cmap(nz))

            # x, y, z = particle.position
            # ax.plot([x], [y], [z], 'C0o')

        for triangle in triangles:
            x, y, z = zip(*triangle.vertices())
            ax.plot(x, y, z, '--', color='0.5')
        ax.set_xlim([0,sim.L])
        ax.set_ylim([0,sim.L])
        ax.set_zlim([0,sim.L])
        plt.savefig(args['--plot'])


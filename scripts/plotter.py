import sys

import numpy as np
from matplotlib import cm
from matplotlib import pyplot as plt

class Particle:

    def __init__(self, d, x, y, z, i):
        self.diameter = d
        self.position = np.array([x, y, z], dtype=np.float64)
        self.i = i

def read_csv(path):
    with open(path) as csvf:
        lines = csvf.readlines()

    L = float(lines[0])
    particles = list()
    for line in lines[1:]:
        data = (float(v) for v in line.split(','))
        particles.append(Particle(*data, len(particles)))
    return L, particles


if __name__ == "__main__":

    print('plotting')

    args = sys.argv[1:]

    assert len(args) == 2

    L, particles = read_csv(args[0])
    last = len(particles)-1
    print(last)

    fig, axes = plt.subplots(nrows=3)
    cmap = cm.get_cmap('viridis', 12)
    for ax in axes:
        ax.set_ylim([-1, L+1])
        ax.set_xlim([-1, L+1])

    for i, ax in enumerate(axes):
        particles = list(sorted(particles, key=lambda p:p.position[i]))
        for particle in particles:
            xy = list(particle.position)
            z = xy.pop(i)
            c = cmap(z/L)
            if particle.i == last:
                c= 'magenta'
            ax.add_patch(plt.Circle(xy, particle.diameter/2.0, ec='black', fc=c))
    plt.savefig(args[1])


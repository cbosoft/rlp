import sys

import numpy as np
from matplotlib import cm
from matplotlib import pyplot as plt

from particle import Particle
from csv import read_csv



def dr(p1, p2):
    dr = np.subtract(p1, p2)
    return np.sqrt(np.dot(dr, dr))



if __name__ == "__main__":

    print('plotting')

    args = sys.argv[1:]

    assert len(args) == 2

    L, vf, ff, particles = read_csv(args[0])
    n = len(particles)
    last = n-1
    print(n)
    interested_particle = particles[last]
    interested_position = interested_particle.position
    #particles.append(Particle(1.0, 3.07002, 9.87032-10.0, 0, 1000))

    fig, axes = plt.subplots(ncols=3, subplot_kw=dict(aspect='equal'), figsize=(15,5))
    titles = ['Right: -X -> YZ', 'Front: Y -> XZ', 'Top: -Z -> XY']
    dirs = ['X', 'Y', 'Z']


    cmap = cm.get_cmap('viridis', 12)
    for i, (ax, title) in enumerate(zip(axes, titles)):
        ax.set_ylim([-1, L+1])
        ax.set_xlim([-1, L+1])
        ax.plot([0],[0], color='white')
        ax.set_title(title)
        xy = list(dirs)
        z = xy.pop(i)
        x, y = xy
        ax.set_xlabel(x)
        ax.set_ylabel(y)

    for i, ax in enumerate(axes):
        particles = list(sorted(particles, key=lambda p:p.position[i]))
        for particle in particles:

            alpha = 1.0
            if dr(particle.position, interested_position) > 1.1:
                alpha = 0.5

            xy = list(particle.position)
            z = xy.pop(i)
            c = cmap(z/L)
            if particle == interested_particle:
                c = 'magenta'
            ax.add_patch(plt.Circle(xy, particle.diameter/2.0, ec='black', fc=c, alpha=alpha))

    # print(dr([3.07002, 9.87032-10.0, 0], [3.52381, 1.62458, 0]))
    # plt.sca(axes[2])
    # x, y, z = zip([3.07002, 9.87032-10.0, 0], [4.05775, 1.36757, 0.805514], [3.52381, 1.62458, 0])
    # x = list(x); y = list(y)
    # x.append(x[0])
    # y.append(y[0])
    # plt.plot(x, y)
    Zmin = np.median([p.Z for p in particles])
    Zav = np.mean([p.Z for p in particles])
    Zmax = np.max([p.Z for p in particles])
    print(Zmin, Zav, Zmax)
    granular_rlp = Zav/(Zav + (2.0*np.sqrt(3)))
    plt.text(0.5, 0.9, f'Configuration of {n} particles, $\\phi = {vf:.3f}$, $f = {ff:.3f}$, $\\phi_{{rlp,granular}} = {granular_rlp:.3f}$', transform=fig.transFigure, ha='center', size='xx-large')
    plt.savefig(args[1])


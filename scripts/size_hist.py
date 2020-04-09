from sys import argv

import numpy as np
from matplotlib import pyplot as plt

from csv import read_csv

if __name__ == "__main__":
    args = argv[1:]

    assert len(args) == 2

    L, vf, ff, particles = read_csv(args[0])

    sizes = [p.diameter for p in particles]

    count, edges = np.histogram(sizes, bins='auto')
    sizes = np.average([edges[:-1], edges[1:]], axis=0)
    probability = np.divide(count, len(particles))
    plt.plot(sizes, probability)
    plt.ylabel('Probability $\\mathcal{P}(a)$')
    plt.xlabel('Particle Diameter $a$')
    plt.savefig(args[1])

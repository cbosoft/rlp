from sys import argv

import numpy as np
from matplotlib import pyplot as plt

from csv import read_csv

if __name__ == "__main__":
    args = argv[1:]

    assert len(args) == 2

    L, vf, ff, particles = read_csv(args[0])

    contact_numbers = [p.Z for p in particles]

    count, edges = np.histogram(contact_numbers, bins=list(range(15)))
    contact_numbers = np.average([edges[:-1], edges[1:]], axis=0)
    probability = np.divide(count, len(particles))

    plt.plot(contact_numbers, probability)
    plt.ylabel('Probability $\\mathcal{P}(Z)$')
    plt.xlabel('Contact number $Z$')
    plt.savefig(args[1])

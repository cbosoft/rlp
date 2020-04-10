import numpy as np


class Particle:

    def __init__(self, d, x, y, z, Z, i):
        self.diameter = d
        self.position = np.array([x, y, z], dtype=np.float64)
        self.i = i
        self.Z = Z # contact number

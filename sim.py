import itertools as itt

import numpy as np

from particle import Particle
from geometry import Triangle, get_distance

class Sim:

    def __init__(self, L):
        self.particles = list()
        self.L = L


    def generate_particle(self, diameter=1.0):

        position = np.multiply(self.L, np.random.random((3)))
        # i = 0
        # while any([self.get_distance2(position, particle) < (diameter+particle.diameter)/2.0 for particle in self.particles]):
        #     position = np.multiply(self.L, np.random.random((3)))
        #     i += 1
        #     if i > 1000:
        #         raise Exception("took too long to generate")

        position[2] = self.L
        new_particle = Particle(position, diameter)
        self.settle(new_particle)

        if new_particle.settled:
            self.particles.append(new_particle)


    def find_triangles(self, thresh):
        triplets = itt.product(self.particles, self.particles, self.particles)
        triangles = list()
        trangles_ids = list()
        for pi, pj, pk in triplets:
            ids = set(sorted([pi.id, pj.id, pk.id]))
            
            if len(ids) < 3:
                continue

            if ids in trangles_ids:
                continue

            t = Triangle(pi.position, pj.position, pk.position)
            c = t.centre()

            # if any([self.get_distance(c, t.a) < thresh,
            #     self.get_distance(c, t.b) < thresh,
            #     self.get_distance(c, t.c) < thresh]):
            #     print('edges too small')
            #     continue

            if any([e > 2.0*thresh for e in t.edge_lengths()]):
                continue

            triangles.append(t)
        return triangles


    def settle(self, particle):
        intersections = list()
        for triangle in self.find_triangles(particle.diameter):
            if triangle.intersects(particle.position, D=2):
                intersections.append(triangle)

        print("settling particle", end='')
        if not intersections:
            particle.settle_to(position_z=0.0)
            print(" on floor")
        else:
            # TODO select proper height
            particle.settle_to(position_z=1.0)
            print(" on top")



    def get_distance(self, p1, p2):
        if isinstance(p1, Particle):
            p1 = p1.position
        if isinstance(p2, Particle):
            p2 = p2.position
        return get_distance(p1, p2)


    def get_distance2(self, p1, p2):
        if isinstance(p1, Particle):
            p1 = p1.position
        if isinstance(p2, Particle):
            p2 = p2.position
        return get_distance(p1[:-1], p2[:-1])

import itertools as itt

import numpy as np

from particle import Particle
from geometry import Triangle, get_distance

class Sim:

    def __init__(self, L, verbose=True, log_file='rlp_log.txt'):
        self.particles = list()
        self.L = L
        self.triangles = list()
        self.verbose = verbose
        self.logf = log_file


    def generate_particle(self, diameter=1.0):
        self.log(f'generating particle {len(self.particles)} ', end='')
        position = np.multiply(self.L, np.random.random((3)))
        position[2] = self.L
        self.add_particle(position, diameter)


    def add_particle(self, position, diameter=1.0):
        new_particle = Particle(position, diameter)
        self.settle(new_particle)

        if not new_particle.settled:
            raise Exception('settling failed')

        self.update_triangles()
        self.particles.append(new_particle)


    def update_triangles(self):
        if not self.particles:
            return
        pairs = itt.product(self.particles[:-1], self.particles[:-1])
        pi = self.particles[-1]
        thresh = pi.diameter
        triangles_ids = list()
        for pj, pk in pairs:
            ids = set(sorted([pi.id, pj.id, pk.id]))
            
            if len(ids) < 3:
                continue

            if ids in triangles_ids:
                continue
            triangles_ids.append(ids)

            t = Triangle(pi.position, pj.position, pk.position)
            c = t.centre()

            # if any([e > 2.0*thresh for e in t.edge_lengths()]):
            #     continue
            if np.any(t.thetas > np.pi/2.0):
                continue

            if np.any(get_distance(t.centre(), t.vertices()) > thresh):
                continue

            self.triangles.append(t)


    def settle(self, particle):
        intersections = list()
        for triangle in self.triangles:
            if triangle.intersects(particle.position, D=2):
                intersections.append(triangle)

        self.log(f'{len(self.triangles)} setting', end='')
        if not intersections:
            # TODO not falling into a triangle means the particle will not be supported, but doesn't mean it won't fall.
            # check for nearest neighbours, if they have an XY-separation less than radius, then will tumble away
            particle.settle_to(position_z=0.0)
            self.log(' on floor')
        else:
            # TODO select proper height
            particle.settle_to(position_z=1.0)
            # TODO if particle settles in triangle, triangle is blocked - remove from list
            self.log(' on top')


    def log(self, *args, **kwargs):
        if self.verbose:
            print(*args, **kwargs)
        with open (self.logf, 'a') as f:
            f.write(f'{" ".join(args)}\n')

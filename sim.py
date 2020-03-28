import itertools as itt

import numpy as np

from particle import Particle
from geometry import Triangle, get_distance

class Sim:

    def __init__(self, L, verbose=1, log_file='rlp_log.txt'):
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
        new_particle = Particle(position, diameter, len(self.particles))
        self.settle(new_particle)

        if not new_particle.settled:
            raise Exception('settling failed')

        self.particles.append(new_particle)
        self.update_triangles()


    def update_triangles(self):
        if len(self.particles) < 3:
            return
        pairs = itt.product(self.particles[:-1], self.particles[:-1])
        others = self.particles[:-1]
        pi = self.particles[-1]
        others = [o for o in others if get_distance(pi.position, o.position) < (o.diameter + pi.diameter)*2.0 ]
        pairs = itt.product(others, others)
        others = self.particles[:-1]
        triangles_ids = list()
        for pj, pk in pairs:
            ids = set(sorted([pi.id, pj.id, pk.id]))
            
            if len(ids) < 3:
                self.log(f'no because same ids: {pi.id} {pj.id} {pk.id}', verbosity_minimum=2)
                continue

            if ids in triangles_ids:
                self.log(f'no because repeat', verbosity_minimum=2)
                continue

            triangles_ids.append(ids)

            t = Triangle(pi.position, pj.position, pk.position, pi.diameter, pj.diameter, pk.diameter)
            c = t.centre()
            v = t.vertices()

            if np.any(t.thetas > np.pi/2.0):
                self.log(f'no because obtuse: any({t.thetas} > {np.pi/2.0})', verbosity_minimum=2)
                continue

            d = np.add(np.multiply(t.diameters, 0.5), pi.diameter)
            cv_sep = [get_distance(c, vi) for vi in v]
            if np.any(cv_sep > d):
                self.log(f'no because distant: any({cv_sep} > {d})', verbosity_minimum=2)
                continue

            # TODO: discount triangle if too steep (i.e. zangle lowest vertex to highest > pi/4)

            self.triangles.append(t)


    def settle(self, particle):
        intersections = list()
        for triangle in self.triangles:
            if triangle.intersects(particle.position, D=2):
                intersections.append(triangle)

        self.log(f'{len(self.triangles)} setting', end='')
        if not intersections:
            # TODO not falling into a triangle means the particle will not be 
            # supported, but doesn't mean it'll reach the ground. Check for 
            # nearest neighbours, if they have an XY-separation less than 
            # radius, then will tumble away i.e. move to side and try to find 
            # supporting triangle. If no particles are below, settle to floor
            particle.settle_to(position_z=0.0)
            self.log(' on floor')
        else:

            # sort triangles by z position
            triangles = list(sorted(self.triangles, key=lambda t: max([v[2] for v in t.vertices()])))

            # remove triangles blocked by higher triangles
            for triangle in triangles[1:]:
                self.triangles.pop(self.triangles.index(triangle))

            triangle = triangles[0]
            v = triangle.vertices()

            # this bit is a bit mathsy. We've got three spheres 
            # (radius = radius vertex + radius settling) centred on the 
            # vertices. The new particle position will be the highest
            # intersection point: let's call this point S.
            #
            # We can simplify the calculation as we already know the 
            # x-y coords of the intersection point: the center coords of the 
            # triangle.
            #
            # Sx = Cx; Sy = Cy
            S = triangle.centre()
            #
            # The z coordinate can be found by solving the equation for a 
            # sphere about one of the vertices with the constraints in place.
            # The result is a quadratic, solved by quadtratic formula, with 
            # two results: the top intersection and the bottom.
            a = 1.0
            b = -1.0*v[0][2]
            c = -(particle.diameter + triangle.diameters[0])
            for d in [0,1]:
                B = (v[1][d] + v[2][d] - 2*v[0][d]) / 3.0
                c += B**2.0
            c += v[0][2]**2.0
            sqrtdisc = ((b**2.0) - 4*a*c)**0.5
            S[2] = max([(-b + sqrtdisc)/(2*a), (-b - sqrtdisc)/(2*a)])
            particle.settle_to(position=S)

            self.triangles.pop(self.triangles.index(triangle))
            self.log(' on top')


    def log(self, *args, verbosity_minimum=1, **kwargs):
        if self.verbose >= verbosity_minimum:
            print(*args, **kwargs)
        with open (self.logf, 'a') as f:
            f.write(f'{" ".join(args)}\n')

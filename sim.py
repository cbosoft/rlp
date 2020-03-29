import itertools as itt

import numpy as np

from particle import Particle
from geometry import Vertex, Line, Triangle, get_distance

class Sim:

    def __init__(self, L, verbose=1, log_file='rlp_log.txt', seed=1):
        self.particles = list()
        self.L = L
        self.triangles = list()
        self.lines = list()
        self.verbose = verbose
        self.logf = log_file
        np.random.seed(seed)


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
        self.update_geometry()


    def update_geometry(self):

        if len(self.particles) < 2:
            return

        others = self.particles[:-1]
        pi = self.particles[-1]
        others = [o for o in others if get_distance(pi.position, o.position) < (o.diameter + pi.diameter)*2.0 ]
        lines_ids = list()
        for pj in others:
            if pi.id == pj.id:
                continue

            ids = set(sorted([pi.id, pj.id]))
            if ids in lines_ids:
                continue
            lines_ids.append(ids)
            self.lines.append(Line(pi.position, pj.position, pi.diameter, pj.diameter))

        if len(self.particles) < 3:
            return

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

        self.log(f'{len(self.triangles)} {len(self.lines)} setting', end='')
        if not intersections:

            # Not falling into a triangle means the particle will not be
            # supported, but doesn't mean it'll reach the ground. Check for
            # nearest neighbours, if they have an XY-separation less than
            # radius, then will tumble away i.e. move to side and try to find
            # supporting triangle. If no particles are below, settle to floor
            
            for line in self.lines:
                if line.intersects(particle.position, particle.diameter, D=2):
                    intersections.append(line)

            if not intersections:

                # finally, see if the falling particle will hit a single other particle
                for other in self.particles:
                    v = Vertex(other.position, other.diameter)
                    if v.intersects(particle.position, particle.diameter, D=2):
                        intersections.append(v)

                if not intersections:

                    particle.settle_to(position_z=0.0)
                    self.log(' on floor')

                else:
                    # sort vertices by z position
                    vertices = list(sorted(intersections, key=lambda ve: ve.vertex[2]))

                    # particle moves out of range of vertex
                    particle.position = vertices[0].tumble(particle.position, particle.diameter)

                    # and continues settling
                    self.settle(particle)
                    return

            else:
                # sort lines by z position
                lines = list(sorted(intersections, key=lambda l: max([v[2] for v in l.vertices])))

                # particle moves out of range of line
                particle.position = lines[0].tumble(particle.position, particle.diameter)

                # and continues settling
                self.settle(particle)
                return
        else:

            # sort triangles by z position
            triangles = list(sorted(intersections, key=lambda t: max([v[2] for v in t.vertices()])))

            # remove triangles blocked by higher triangles
            for triangle in triangles[1:]:
                self.triangles.pop(self.triangles.index(triangle))

            triangle = triangles[0]
            particle.settle_to(position=triangle.tumble(particle.diameter))

            self.triangles.pop(self.triangles.index(triangle))
            self.log(' on top')


    def log(self, *args, verbosity_minimum=1, **kwargs):
        if self.verbose >= verbosity_minimum:
            print(*args, **kwargs)
        with open (self.logf, 'a') as f:
            f.write(f'{" ".join(args)}\n')

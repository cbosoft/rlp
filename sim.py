import itertools as itt

import numpy as np

from particle import Particle
from geometry import Vertex, Line, Triangle, get_distance, get_internal_angle, ZAXIS
from exception import RLPError, OutOfBoundsError, RecursionDepthError, GoingInCirclesError, IntersectionError


class Sim:

    def __init__(self, L, verbose=1, log_file='rlp_log.txt', seed=1):
        self.particles = list()
        self.L = L
        self.triangles = list()
        self.lines = list()
        self.vertices =list()
        self.verbose = verbose
        self.logf = log_file
        self.previous_settling_positions = list()

        # set up numpy stuff
        np.random.seed(seed)
        np.seterr(all='raise')


    def generate_particle(self, diameter=1.0):
        self.log(f'generating particle {len(self.particles)} ', end='')
        position = np.multiply(self.L, np.random.random((3)))
        position[2] = self.L

        try:
            self.add_particle(position, diameter)
        except FloatingPointError:
            self.generate_particle(diameter=diameter)
        except IntersectionError:
            for i in self.last_list_interactions:
                print(i)
            raise
        except RLPError:
            self.generate_particle(diameter=diameter)


    def add_particle(self, position, diameter=1.0):
        new_particle = Particle(position, diameter, len(self.particles))
        self.previous_settling_positions = list()
        self.settle(new_particle)

        if not all([0 <= p <= self.L for p in new_particle.position[:2]]):
            raise OutOfBoundsError("OOB")

        for particle in self.particles:
            if particle.intersects_with(new_particle):
                print(particle.position, new_particle.position)
                raise IntersectionError('Particle-particle intersection')

        try:
            self.particles.append(new_particle)
            self.update_geometry()
        except Exception as e:
            self.particles.pop(-1)
            raise


    def update_geometry(self):

        pi = self.particles[-1]
        self.vertices.append(Vertex(pi.position, pi.diameter))

        if len(self.particles) < 2:
            return

        others = self.particles[:-1]

        others = [o for o in others if get_distance(pi.position, o.position) < (o.diameter + pi.diameter)*2.0 ]
        lines_ids = list()
        for pj in others:
            if pi.id == pj.id:
                continue

            ids = set(sorted([pi.id, pj.id]))
            if ids in lines_ids:
                continue

            z_angles = [
                    get_internal_angle(ZAXIS, np.subtract(pj.position, pi.position)),
                    get_internal_angle(ZAXIS, np.subtract(pi.position, pj.position))
                    ]
            if any([za <= np.pi/4.0 for za in z_angles]):
                self.log(f'no because steep: {min(z_angles)} <= {np.pi/4.0})', verbosity_minimum=2)
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

            if t.trilaterate(1.0) is None:
                continue

            self.triangles.append(t)


    def settle(self, particle, n=0, recursion_limit=100, history_limit=5):

        if n > recursion_limit:
            raise RecursionDepthError('Exceeded recursion limit.')

        lpos = list(particle.position)
        if lpos in self.previous_settling_positions:
            raise GoingInCirclesError(f'Going in circles: found current position in recent history ({lpos} in {self.previous_settling_positions}).')

        self.previous_settling_positions.append(lpos)
        if len(self.previous_settling_positions) > history_limit:
            self.previous_settling_positions.pop(0)

        interactions = list()
        for triangle in self.triangles:
            if triangle.could_interact_with(particle):
                interactions.append(triangle)

        for line in self.lines:
            if line.could_interact_with(particle):
                interactions.append(line)

        for vertex in self.vertices:
            if vertex.could_interact_with(particle):
                interactions.append(vertex)

        flpos = '[' + ', '.join([f'{p:.2f}' for p in particle.position]) + ']'
        self.log(f'{flpos}->', end='')
        interactions = list(filter(lambda i: i.get_lowpoint() < particle.position[2], interactions))

        if not interactions:
            particle.settle_to(position_z=0.0)
            self.log('FLOOR')
            return

        self.last_list_interactions = list(sorted(interactions, key=lambda i: i.get_sortkey(particle)))
        interaction = sorted(interactions, key=lambda i: i.get_sortkey(particle))[0]
        interaction.interact(particle)

        if not particle.settled:
            self.settle(particle, n=n+1)
        else:
            assert isinstance(interaction, Triangle)

            self.triangles.pop(self.triangles.index(interaction))
            flpos = '[' + ', '.join([f'{p:.2f}' for p in particle.position]) + ']'
            self.log(f'{flpos}')


    def log(self, *args, verbosity_minimum=1, **kwargs):
        if self.verbose >= verbosity_minimum:
            print(*args, **kwargs)
        with open (self.logf, 'a') as f:
            f.write(f'{" ".join(args)}\n')

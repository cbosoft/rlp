import numpy as np

from geometry import Vertex, Line, Triangle
from sim import Sim
from colours import *

QUIET = False

# Test-driven-development inspired by "Clean Code" - Uncle Bob.

def approx_eq(a, b, eps=1e-7):
    return a-eps <= b <= a+eps

def vector_approx(vector1, vector2, eps=1e-7):
    return all([vj-eps < vi < vj+eps for vi,vj in zip(vector1, vector2)])


class TestFailure(Exception):
    '''test failed.'''

class Test:


    critical = True
    name = 'BASE TEST'


    def fail_test(self, extra=None):
        m = self.name
        if extra:
            m += f' {extra} '
        m += f' {BG_RED}FAILED{RESET}'
        if self.critical:
            raise TestFailure(m)
        elif not QUIET:
            print(m)


    def get_name(self):
        return self.name


    def pass_test(self):
        if not QUIET:
            print(f'{self.get_name()} {FG_GREEN}PASSED{RESET}')


    def run(self):
        raise NotImplementedError('func not implemented in base; use derived')




class MultiTest(Test):

    i = 0
    points = list()


    def get_name(self):
        return f'{self.name} ({self.i+1}/{len(self.points)})'


    def run(self, i):
        raise NotImplementedError('func not implemented in base; use derived')


    def run_each(self):
        for i in range(len(self.points)):
            self.i = i
            self.run(i)




class VertexTestIntersect2D(MultiTest):

    name = 'Vertex intersection test (2D)'
    points = [
            [0.5, 0.5, 0.0],
            [2.0, 2.0, 0.0],
            [0.5, 0.5, 10.0]
        ]
    results = [
            True,
            False,
            True
        ]

    def run(self, i):
        # build
        v = Vertex([0.0, 0.0, 0.0], 1.0)

        # operate
        p = self.points[i]
        expected_result = self.results[i]
        res = v.intersects(p, 1.0, D=2)
        n = '' if expected_result else 'n\'t'
        nn = 'n\'t' if expected_result else ''
        if res == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'{p} should{n} intersect {v}, but did{nn}.')



class VertexTestIntersect3D(MultiTest):

    name = 'Vertex intersection test (3D)'
    points = [
            [0.5, 0.5, 0.0],
            [2.0, 2.0, 0.0],
            [0.5, 0.5, 10.0]
        ]
    results = [
            True,
            False,
            False
        ]

    def run(self, i):
        # build
        v = Vertex([0.0, 0.0, 0.0], 1.0)

        # operate
        p = self.points[i]
        expected_result = self.results[i]
        res = v.intersects(p, 1.0)
        n = '' if expected_result else 'n\'t'
        nn = 'n\'t' if expected_result else ''
        if res == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'{p} should{n} intersect {v}, but did{nn}.')


class VertexTestTumble(MultiTest):

    name = 'Vertex tumble test'
    points = [
            [0.5, 0.0, 0.0],
            [0.0, 0.5, 0.0],
            [0.5, 0.5, 0.0]
        ]
    results = [
            [1.0, 0.0, 0.0],
            [0.0, 1.0, 0.0],
            [np.sqrt(0.5), np.sqrt(0.5), 0.0]
        ]

    def run(self, i):
        # build
        v = Vertex([0.0, 0.0, 0.0], 1.0)

        # operate
        p = self.points[i]
        expected_result = self.results[i]
        result = v.tumble(p, 1.0)
        n = '' if expected_result else 'n\'t'
        nn = 'n\'t' if expected_result else ''
        if vector_approx(result, expected_result):
            self.pass_test()
        else:
            self.fail_test(f'{p} should{n} settle to {expected_result}, but did{nn} ({result}).')










class LineIntersectionTest2D(MultiTest):

    name = 'Line intersection test (2D)'
    points = [
            [0.75, 0.0, 0.0],
            [5.0, 5.0, 5.0],
            [0.75, 0.0, 10.0]
        ]
    results = [
            True,
            False,
            True
        ]

    def run(self, i):
        l = Line([0.0, 0.0, 0.0], [1.5, 0.0, 0.0], 1.0, 1.0)
        p = self.points[i]
        expected_result = self.results[i]
        res = l.intersects(p, 1.0, D=2)
        n = '' if expected_result else 'n\'t'
        nn = 'n\'t' if expected_result else ''
        if res == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'{p} should{n} intersect {l}, but did{nn}.')




class LineIntersectionTest3D(MultiTest):

    name = 'Line intersection test (3D)'
    points = [
            [0.75, 0.0, 0.0],
            [5.0, 5.0, 5.0],
            [0.75, 0.0, 10.0]
        ]
    results = [
            True,
            False,
            False
        ]

    def run(self, i):
        l = Line([0.0, 0.0, 0.0], [1.5, 0.0, 0.0], 1.0, 1.0)
        p = self.points[i]
        expected_result = self.results[i]
        res = l.intersects(p, 1.0)
        n = '' if expected_result else 'n\'t'
        nn = 'n\'t' if expected_result else ''
        if res == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'{p} should{n} intersect {l}, but did{nn}.')














class TriangleAreaTest(Test):

    name = 'Triangle area test'

    def run(self):
        # Build
        t = Triangle([0, 0, 0], [1, 0, 0], [0, 1, 0], 1.0, 1.0, 1.0)

        # Operate and Check
        if (a:= t.area()) != 0.5:
            self.fail_test(f'({a} != 0.5)')
        else:
            self.pass_test()


class TriangleCentreTest(Test):

    name = 'Triangle centre test'

    def run(self):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        if (c := list(t.centre())) != (cp := [0.0, 1.0/3.0, 0.0]):
            self.fail_test(f'({c} != {cp})')
        else:
            self.pass_test()


class TriangleIntersectTest3D(MultiTest):

    name = 'Triangle point intersection test (3D)'
    points = [
            [0.0, 0.5, 0.0],
            [0.0, 0.5, 1.0],
            [-5, -5, -5],
            [-1, 0.9, 0.0]
        ]
    outcomes = [
            True,
            False,
            False,
            False
        ]

    def run(self, i):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[i]
        expected_result = self.outcomes[i]
        result = t.intersects(p)
        n = '' if expected_result else 'n\'t'
        if result == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'({p} should{n} intersect {t})')


class TriangleIntersectTest2D(MultiTest):

    name = 'Triangle point intersection test (2D)'
    points = [
            [0.0, 0.5, 19.0],
            [-5, -5, 100],
            [-1, 0.9, 1.0]
        ]
    outcomes = [
            True,
            False,
            False
        ]

    def run(self, i):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[i]
        expected_result = self.outcomes[i]
        result = t.intersects(p, D=2)
        n = '' if expected_result else 'n\'t'
        if result == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'({p} should{n} intersect {t})')

class SimAddParticleTestCount(Test):
    name = 'Sim add particle test (count)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 0.0])
        sim.add_particle([0.0, 2.0, 0.0])
        sim.add_particle([2.0, 0.0, 0.0])

        # check
        if (l := len(sim.particles)) != 3:
            self.fail_test(f'number particles ({l}) != 3')
        else:
            self.pass_test()

class SimAddParticleTestTriangle(Test):
    name = 'Sim add particle test (triangle)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 0.0])
        sim.add_particle([0.0, 2.0, 0.0])
        sim.add_particle([2.0, 0.0, 0.0])

        # check
        if (l := len(sim.triangles)) != 1:
            self.fail_test(f'number triangles ({l}) != 1')
        else:
            self.pass_test()

class SimAddParticleTestIntersection(Test):
    name = 'Sim add particle test (intersection)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        # first three form a triangle
        sim.add_particle([0.0, 0.0, 10.0])
        sim.add_particle([0.0, 1.0, 10.0])
        sim.add_particle([1.0, 0.0, 10.0])
        cx, cy, cz = sim.triangles[-1].centre()

        # last one should settle on the triangle
        sim.add_particle([0.5, 0.5, 10.0])

        # check
        sx, sy, sz = sim.particles[-1].position
        if sx != cx or sy != cy:
            self.fail_test(f'settled xy not centre ({[sx, sy]} != {cx, cy})')
        elif not approx_eq(sz, 1.2, 0.05):
            self.fail_test(f'settled z not correct height ({sz} !~ {1.2})')
        else:
            self.pass_test()






def run_tests(quiet=False):
    global QUIET
    QUIET = quiet

    # Vertex
    VertexTestIntersect2D().run_each()
    VertexTestIntersect2D().run_each()
    VertexTestTumble().run_each()

    # Line
    LineIntersectionTest2D().run_each()
    LineIntersectionTest3D().run_each()

    # Triangle
    TriangleAreaTest().run()
    TriangleCentreTest().run()
    TriangleIntersectTest3D().run_each()
    TriangleIntersectTest2D().run_each()

    # Sim
    SimAddParticleTestCount().run()
    SimAddParticleTestTriangle().run()
    SimAddParticleTestIntersection().run()

if __name__ == "__main__":
    run_tests()

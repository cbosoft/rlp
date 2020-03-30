import numpy as np

from geometry import Vertex, Line, Triangle
from sim import Sim
from colours import *
from exception import OutOfBoundsError

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

        # check
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
        conf = v.intersects(result, 1.0)
        if conf:
            self.fail_test(f'{result} should be out-of-range of {v}, but it isn\'t.')

        # check
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


class LineTumbleTest(MultiTest):

    name = 'Line tumble test'
    points = [
            [0.5,  0.1, 0.0],
            [0.5, -0.1, 0.0],
        ]
    results = [
            [0.5,  0.866, 0.0],
            [0.5, -0.866, 0.0]
        ]

    def run(self, i):
        # build
        l = Line([0.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0)
        p = self.points[i]
        expected_result = self.results[i]

        # operate
        result = l.tumble(p, 1.0)

        # check
        if vector_approx(result, expected_result, 1e-3):
            self.pass_test()
        else:
            self.fail_test(f'({result} !~ {expected_result})')














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

    name = 'Triangle test (centre)'

    def run(self):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        if (c := list(t.centre())) != (cp := [0.0, 1.0/3.0, 0.0]):
            self.fail_test(f'({c} != {cp})')
        else:
            self.pass_test()



class TriangleIntersectTest2D(MultiTest):

    name = 'Triangle point intersection test (2D)'
    points = [
            [0.5, 0.5, 19.0],
            [-5, -5, 100],
            [-1, 0.9, 1.0]
        ]
    outcomes = [
            True,
            False,
            False
        ]

    def run(self, i):
        t = Triangle(
                [0.0, 0.0, 0.0], 
                [1.0, 0.0, 0.0], 
                [0.0, 1.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[i]
        expected_result = self.outcomes[i]
        result = t.intersects(p, 1.0, D=2)
        n = '' if expected_result else 'n\'t'
        if result == expected_result:
            self.pass_test()
        else:
            self.fail_test(f'({p} should{n} intersect {t})')


class TriangleIntersectTest3D(MultiTest):

    name = 'Triangle point intersection test (3D)'
    points = [
            [0.5, 0.5, 0.0],
            [0.5, 0.5, 1.0],
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
        t = Triangle(
                [0.0, 0.0, 0.0],
                [1.0, 0.0, 0.0],
                [0.0, 1.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[i]
        expected_result = self.outcomes[i]
        result = t.intersects(p, 1.0)
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


class SimAddParticleTestLine(Test):
    name = 'Sim add particle test (line)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 0.0])
        sim.add_particle([0.0, 2.0, 0.0])

        # check
        if (l := len(sim.lines)) != 1:
            self.fail_test(f'number lines ({l}) != 1')
        else:
            self.pass_test()


class SimAddParticleTestTriangle(Test):
    name = 'Sim add particle test (triangle)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 0.0])
        sim.add_particle([0.0, 1.0, 0.0])
        sim.add_particle([1.0, 0.0, 0.0])

        # check
        if (l := len(sim.triangles)) != 1:
            self.fail_test(f'number triangles ({l}) != 1')
        else:
            self.pass_test()


class SimAddParticleTestTumbleVertex(Test):
    name = 'Sim add particle test (tumble) (vertex)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 10.0])
        sim.add_particle([0.5, 0.0, 10.0])

        # check
        p = sim.particles[-1].position
        if np.any(p != [1.0, 0.0, 0.0]):
            self.fail_test(f'settled position not correct ({p} != [1.0, 0.0, 0.0])')
        else:
            self.pass_test()


class SimAddParticleTestTumbleLine(Test):
    name = 'Sim add particle test (tumble) (line)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        sim.add_particle([0.0, 0.0, 10.0])
        sim.add_particle([1.0, 0.0, 10.0])

        sim.add_particle([0.5, 0.1, 10.0])

        # check
        result = sim.particles[-1].position
        expected_result = [0.5, (1.0 - 0.5*0.5)**0.5, 0.0]
        if not vector_approx(result, expected_result):
            self.fail_test(f'settled position not correct ({result} != {expected_result})')
        else:
            self.pass_test()


class SimAddParticleTestIntersectionTriangle(Test):
    name = 'Sim add particle test (intersection) (triangle)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        # first three form a triangle
        sim.add_particle([0.0, 0.0, 10.0])
        sim.add_particle([0.0, 1.0, 10.0])
        sim.add_particle([1.0, 0.0, 10.0])

        # last one should settle on the triangle
        sim.add_particle([0.49, 0.5, 10.0])

        # check
        result = sim.particles[-1].position
        expected_result = [0.5, 0.5, np.sqrt(0.5)]
        if not vector_approx(result, expected_result):
            self.fail_test(f'settled pos not correct ({result} !~ {expected_result})')
        else:
            self.pass_test()


class SimAddParticleTestFarTriangle(Test):
    name = 'Sim add particle test (far triangle)'
    points = [
            [3.91670032, 0.4896196, 0.0],

            [2.3668456, 0.71321559, 0.0],
            [3.02332573, 1.4755891, 0.0]
        ]

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=10)
        for point in self.points:
            sim.add_particle(point)

        # check
        if (l := len(sim.triangles)) != 1:
            self.fail_test(f'number triangles ({l}) != 1')
        else:
            self.pass_test()

class SimAddParticleTestFarTrianlgleTumble(SimAddParticleTestFarTriangle):
    name = 'Sim add particle test (far triangle) (tumble)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=0)
        for point in self.points:
            sim.add_particle(point)

        p = [2.9180535, 0.5462543, 10.0]
        t = sim.triangles[-1]
        result = False
        try:
            sim.add_particle(p)
            result = False
        except OutOfBoundsError:
            result = True

        # check
        expected_result = True
        if expected_result == result:
            self.pass_test()
        else:
            self.fail_test(f'settled pos not out of bounds as expected.')






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
    LineTumbleTest().run_each()

    # Triangle
    TriangleAreaTest().run()
    TriangleCentreTest().run()
    TriangleIntersectTest2D().run_each()
    TriangleIntersectTest3D().run_each()

    # Sim
    SimAddParticleTestCount().run()
    SimAddParticleTestLine().run()
    SimAddParticleTestTriangle().run()
    SimAddParticleTestTumbleVertex().run()
    SimAddParticleTestTumbleLine().run()
    SimAddParticleTestIntersectionTriangle().run()

    SimAddParticleTestFarTriangle().run()
    SimAddParticleTestFarTrianlgleTumble().run()
    print("done!")

if __name__ == "__main__":
    run_tests()


from geometry import Triangle
from sim import Sim
from colours import *


# Test-driven-development inspired by "Clean Code" - Uncle Bob.

def approx_eq(a, b, eps=1e-7):
    return a-eps <= b <= a+eps


class TestFailure(Exception):
    '''test failed.'''

class Test:

    critical = False
    name = 'BASE TEST'

    def fail_test(self, extra=None):
        m = self.name
        if extra:
            m += f' {extra} '
        m += f' {BG_RED}FAILED{RESET}'
        if self.critical:
            raise TestFailure(m)
        else:
            print(m)

    def pass_test(self):
        print(f'{self.name} {FG_GREEN}PASSED{RESET}')

    def run(self):
        raise NotImplementedError('func not implemented in base; use derived')


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


class TriangleIntersectTest3D(Test):

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

    def __init__(self, i=0):
        self.i = i

    def run(self):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[self.i]
        o = self.outcomes[self.i]
        i = t.intersects(p)
        n = '' if o else 'n\'t'
        if i == o:
            self.pass_test()
        else:
            self.fail_test(f'({p} should{n} be in {t})')


class TriangleIntersectTest2D(Test):

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

    def __init__(self, i=0):
        self.i = i

    def run(self):
        t = Triangle([0.0, 1.0, 0.0], [-1.0, 0.0, 0.0], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0)
        p = self.points[self.i]
        o = self.outcomes[self.i]
        i = t.intersects(p, D=2)
        n = '' if o else 'n\'t'
        if i == o:
            self.pass_test()
        else:
            self.fail_test(f'({p} should{n} be in {t})')

class SimAddParticleTestCount(Test):
    name = 'Sim add particle test (count)'

    def run(self):
        # Build and operate
        sim = Sim(10.0, verbose=False)
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
        sim = Sim(10.0, verbose=False)
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






if __name__ == "__main__":
    TriangleAreaTest().run()
    
    TriangleCentreTest().run()
    
    TriangleIntersectTest3D().run()
    TriangleIntersectTest3D(1).run()
    TriangleIntersectTest3D(2).run()
    TriangleIntersectTest3D(3).run()

    TriangleIntersectTest2D().run()
    TriangleIntersectTest2D(1).run()
    TriangleIntersectTest2D(2).run()

    SimAddParticleTestCount().run()
    SimAddParticleTestTriangle().run()
    SimAddParticleTestIntersection().run()

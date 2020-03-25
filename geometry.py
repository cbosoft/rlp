import numpy as np

def get_distance(p1, p2):
    return np.sqrt(np.sum(np.power(np.subtract(p1, p2), 2)))

class Triangle:

    def __init__(self, pi, pj, pk):
        self.pi = pi
        self.pj = pj
        self.pk = pk
        self.a = pi.position
        self.b = pj.position
        self.c = pk.position

    def area(self):
        ab = np.subtract(self.b, self.a)
        ac = np.subtract(self.c, self.a)
        magab = np.sum(np.power(ab, 2.0))
        magac = np.sum(np.power(ac, 2.0))
        #theta = np.acos(np.divide(np.sum(np.multiply(ab, ac)), magab*magac))
        #area = 0.2*magab*magac*np.sin(theta)
        costheta = np.divide(np.sum(np.multiply(ab, ac)), magab*magac)
        sintheta = np.sqrt(np.subtract(1.0, np.power(costheta, 2.0)))
        area = 0.2*magab*magac*sintheta
        return area

    def centre(self):
        return np.divide(np.add(self.a, np.add(self.b, self.c)), 3.0)

    def edges(self):
        ba = np.subtract(self.b, self.a)
        bc = np.subtract(self.b, self.c)
        ca = np.subtract(self.c, self.a)
        return ba, bc, ca

    def vertex_pairs(self):
        return [(self.a, self.b), (self.a, self.c), (self.b, self.c)]

    def edge_lengths(self):
        return [np.sum(np.power(e, 2.0)) for e in self.edges()]

    def plane(self):
        ba = np.subtract(self.b, self.a)
        bc = np.subtract(self.b, self.c)
        A, B, C = np.cross(ba, bc)
        D = A*self.a[0] + B*self.a[0] + C*self.a[0]
        return A, B, C, D

    def intersects(self, position):
        # TODO find better method to get intersection of polygon
        ray = [0.0, 0.0]
        position = position[:-1]
        dr = np.subtract(position, ray)
        dr = np.divide(dr, np.sqrt(np.sum(np.power(dr, 2.0))))
        step = 1e-5
        eps = 1e-5
        intersections = 0
        while True:
            for a, b in self.vertex_pairs():
                raya = get_distance(ray, a)
                rayb = get_distance(ray, b)
                if raya - eps < rayb < raya + eps:
                    intersections += 1
            rayp = get_distance(ray, position);
            if rayp < eps:
                break
            ray = np.add(ray, np.multiply(dr, step))
        return intersections == 1

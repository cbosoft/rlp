import numpy as np

def get_distance(p1, p2):
    return np.sqrt(np.sum(np.power(np.subtract(p1, p2), 2)))

def get_internal_angle(v1, v2):
    mag1 = np.sqrt(np.dot(v1, v1))
    mag2 = np.sqrt(np.dot(v2, v2))
    costheta = np.divide(v1.dot(v2), mag1*mag2)
    return np.arccos(costheta)

class Triangle:
    '''
    Triangle class: a representation of a trio of particles which together form
    a triangle, with particle centres at each of vertices.
    '''

    def __init__(self, a, b, c, da, db, dc):
        self.a = a
        self.b = b
        self.c = c
        vertices = [a, b, c]
        self.diameters = [da, db, dc]
        self.thetas = list()
        self.thetas2d = list()
        for i, vertex in enumerate(vertices):
            vertex = np.array(vertex)
            vectors = [np.subtract(vertex, other) for j, other in enumerate(vertices) if i != j]
            vectors2d = [np.subtract(vertex[:-1], other[:-1]) for j, other in enumerate(vertices) if i != j]
            self.thetas.append(get_internal_angle(*vectors))
            self.thetas2d.append(get_internal_angle(*vectors2d))
        self.thetas = np.array(self.thetas, dtype=np.float64)
        self.thetas2d = np.array(self.thetas2d, dtype=np.float64)

    def __repr__(self):
        return repr(self.vertices())

    def __str__(self):
        return str(self.vertices())



    def area(self):
        ab = np.subtract(self.b, self.a)
        ac = np.subtract(self.c, self.a)
        magab = np.sqrt(np.sum(np.power(ab, 2.0)))
        magac = np.sqrt(np.sum(np.power(ac, 2.0)))
        costheta = np.divide(np.dot(ab, ac), magab*magac)
        sintheta = np.sqrt(np.subtract(1.0, np.power(costheta, 2.0)))
        area = 0.5*magab*magac*sintheta
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


    def vertices(self):
        return np.array([self.a, self.b, self.c], dtype=np.float64)


    def edge_lengths(self):
        return [np.sum(np.power(e, 2.0)) for e in self.edges()]


    def plane(self):
        ba = np.subtract(self.b, self.a)
        bc = np.subtract(self.b, self.c)
        A, B, C = np.cross(ba, bc)
        D = A*self.a[0] + B*self.a[0] + C*self.a[0]
        return A, B, C, D


    def intersects(self, position, D=3):

        # first rough check
        vertices = self.vertices()
        maxv = np.max(vertices, axis=0)
        minv = np.min(vertices, axis=0)
        if np.any(position[:D] > maxv[:D]):
            return False
        if np.any(position[:D] < minv[:D]):
            return False

        # more involved check
        thetas = self.thetas2d if D == 2 else self.thetas
        for vertex, theta in zip(vertices, thetas):
            vertex = np.array(vertex)
            vectors = list()
            for other in vertices:

                if all(other[:D] == vertex[:D]):
                    continue

                vectors.append(np.subtract(vertex[:D], other[:D]))

            v_to_p = np.subtract(vertex, position)
            for vector in vectors:
                ang = get_internal_angle(vector[:D], v_to_p[:D])
                if ang > theta:
                    return False

        return True

import numpy as np

EPSILON = 1e-15

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

        assert D in [2,3]

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

    def tumble(self, diameter):
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
        S = self.centre()
        #
        # The z coordinate can be found by solving the equation for a 
        # sphere about one of the vertices with the constraints in place.
        # The result is a quadratic, solved by quadtratic formula, with 
        # two results: the top intersection and the bottom.
        v = self.vertices()
        a = 1.0
        b = -1.0*v[0][2]
        c = -(diameter + self.diameters[0])
        for d in [0,1]:
            B = (v[1][d] + v[2][d] - 2*v[0][d]) / 3.0
            c += B**2.0
        c += v[0][2]**2.0
        sqrtdisc = ((b**2.0) - 4*a*c)**0.5
        S[2] = max([(-b + sqrtdisc)/(2*a), (-b - sqrtdisc)/(2*a)])
        return S



class Line:

    def __init__(self, p1, p2, d1, d2):
        self.vertices = np.array([p1, p2], dtype=np.float64)
        self.diameters = np.array([d1, d2], dtype=np.float64)


    def __repr__(self):
        return repr(self.vertices)


    def __str__(self):
        return str(self.vertices)


    def intersects(self, position, diameter, D=3):
        assert D in [2,3]
        distances = [get_distance(v[:D], position[:D]) for v in self.vertices]
        return all([distance < (diameter + vdiameter)*0.5 for distance, vdiameter in zip(distances, self.diameters)])


    def tumble(self, position, diameter):
        '''
        A particle (S0) tumbling over a line (R1, R2). The particle will head
        to the centre, and then over to the side it was on until it is out of
        range of the line.

            dS = S0 - R1

            dR = R2 - R1

        Get component of dS parallel to dR:

            TS = ((dS·dR)/(dR·dR))·dR

        Normal component is sum of tangent and main:

            NS = S0 + TS

        Normalise (making a unit vector):

            NS = NS / |NS|

        Get the centre point of the line:

            C = R1 + 0.5(R2 - R1)

        Get the length of the separation between centre C and settling particle
        final position S (finding the length of the triangle side):

            a = max([DR1, DR2]) + DS
            b = |0.5*dR|
            c = (a^2 - b^2)^0.5

        Particle final position is then simply:

            S = C + cNS
        '''
        S0 = position[:2]
        DS = diameter
        DR1, DR2 = self.diameters
        R1, R2 = self.vertices
        R1 = R1[:2]
        R2 = R2[:2]
        dS = S0 - R1
        dR = R2 - R1
        TS = dR*np.dot(dS, dR)/np.dot(dR, dR)
        NS = S0+TS
        NS = np.divide(NS, np.sqrt(np.sum(np.power(NS, 2.0))))
        C = R1 + 0.5*(R2 - R1)
        a = max([DR1, DR2]) + DS
        b = np.sqrt(np.sum(np.power(dR*0.5, 2.0)))
        c = (a*a - b*b)**0.5
        S = C + NS*c
        new_position = np.array(position)
        new_position[:2] = S
        return new_position



class Vertex:

    def __init__(self, p, d):
        self.vertex = np.array(p, dtype=np.float64)
        self.diameter = d


    def __repr__(self):
        return repr(self.vertex)


    def __str__(self):
        return str(self.diameter)


    def intersects(self, position, diameter, D=3):
        '''
        Vertex-particle intersection; occurs if centre-centre distance (dS) is
        less than the average of the two diameters. To stop particles /just/
        in contact being counted, increase the distance by epsilon.
        '''
        assert D in [2,3]
        return get_distance(self.vertex[:D], position[:D]) + EPSILON < (diameter + self.diameter)*0.5


    def tumble(self, position, diameter):
        '''
        A particle (S0) hitting another particle (R) from above will tumble around until
        it is clear of R's diameter.

        The settling particle's new (xy) position will be:

        S = S0 + UNIT(S0 - R)*(Ds + Dr)*0.5
        '''

        # xy separation vector
        ds = np.subtract(position[:2], self.vertex[:2])

        # divide by magnitude: unit vector
        ds = np.divide(ds, np.sqrt(np.sum(np.power(ds, 2.0))))

        # multiply by distance
        ds = np.multiply(ds, (diameter + self.diameter)*0.5)

        new_position = np.copy(position)
        new_position[:2] = np.add(self.vertex[:2], ds)
        return new_position



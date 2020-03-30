import numpy as np

EPSILON = 1e-15

ZAXIS = np.array([0.0, 0.0, 1.0])


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
        self.a = np.array(a, dtype=np.float64)
        self.b = np.array(b, dtype=np.float64)
        self.c = np.array(c, dtype=np.float64)
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


    def intersects(self, position, diameter, D=3):

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
                angle = min([get_internal_angle(vector[:D]*m, v_to_p[:D]) for m in [1.0, -1.0]])
                if angle > theta:
                    return False

        if self.trilaterate(diameter) is None:
            return False

        return True

    def trilaterate(self, diameter):
        '''
        Trilaterate intersection points of three spheres centred on the
        triangles vertices.

        https://stackoverflow.com/a/18654302
        '''
        r1, r2, r3 = np.multiply(np.add(self.diameters, diameter), 0.5)
        temp1 = self.b - self.a
        d = np.linalg.norm(temp1)
        e_x = temp1/d
        temp2 = self.c - self.a
        i = np.dot(e_x, temp2)
        temp3 = temp2 - i*e_x
        e_y = temp3/np.linalg.norm(temp3)
        e_z = np.cross(e_x, e_y)
        j = np.dot(e_y, temp2)
        x = (r1*r1 - r2*r2 + d*d) / (2*d)
        y = (r1*r1 - r3*r3 -2*i*x + i*i + j*j) / (2*j)
        temp4 = r1*r1 - x*x - y*y
        if temp4 < 0:
            return None
        z = np.sqrt(temp4)
        p_12_a = self.a + x*e_x + y*e_y + z*e_z
        p_12_b = self.a + x*e_x + y*e_y - z*e_z
        return p_12_a, p_12_b

    def tumble(self, diameter):
        a, b = self.trilaterate(diameter)
        if a[2] > b[2]:
            return a
        return b



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

        v = self.vertices
        if get_distance(v[0], v[1]) >= diameter + np.average(self.diameters):
            return False

        if all([get_distance(position[:D], vertex[:D])+EPSILON >= (vertex_diameter + diameter)*0.5 for vertex, vertex_diameter in zip(v, self.diameters)]):
            return False

        for vertex, other in zip(self.vertices, self.vertices[::-1]):
            dv = np.subtract(other[:D], vertex[:D])
            dp = np.subtract(position[:D], vertex[:D])
            theta = get_internal_angle(dv, dp)
            if theta >= np.pi/2.0:
                return False

        d = [get_distance(position[:D], vertex[:D])-1e-10 for vertex in v]
        return True



    def tumble(self, position, diameter):
        '''
        A particle (S0) tumbling over a line (R1, R2). The particle will head
        to the centre, and then over to the side it was on until it is out of
        range of the line.

            dS = S0 - R1
            dS' = dS/|dS|

            dR = R2 - R1
            dR' = dR/|dR|

        Get component of dS parallel to dR:

            TS = ((dS·dR)/(dR·dR))·dR

        Normal component is sum of tangent and main:

            NS = S0 + TS

        Normalise (making a unit vector):

            NS' = NS / |NS|

        Get angle between R1->S and R1->R2:

            a = |R1->R2|
            b = |R1->S| = (DR1 + DS)*0.5
            c = |R2->S| = (DR2 + DS)*0.5
            theta = arccos( (a^2 + b^2 - c^2)/(2*a*b) )

        Get the centre point of the line:

            t = |R1->C| = |R1->S|*cos(theta)
            C = R1 + dR'*t

        Get final position:

            l = |C->S| = |R1->S|*sin(theta)
            S = C + NS'*l
        '''
        S0 = position[:2]
        DS = diameter
        DR1, DR2 = self.diameters
        R1, R2 = self.vertices
        R1, R2 = R1[:2], R2[:2]
        dS0 = S0 - R1
        dS0_mag = np.sqrt(np.dot(dS0, dS0))
        dR = R2 - R1
        dR_mag = np.sqrt(np.dot(dR, dR))
        dRu = np.divide(dR, dR_mag)

        TS = dRu*np.dot(dS0, dR)/dR_mag
        NS0 = dS0-TS
        NS0u = np.divide(NS0, np.sqrt(np.dot(NS0, NS0)))

        a = dR_mag
        b = (DR1 + DS)*0.5
        c = (DR2 + DS)*0.5
        costheta = (a*a + b*b - c*c)/(2*a*b)
        theta = np.arccos(costheta)

        dS_mag = (DR1 + DS)*0.5
        t = dS_mag * costheta
        C = np.add(R1, np.multiply(dRu, t))

        l = dS_mag * np.sin(theta)
        S = np.add(C, np.multiply(NS0u, l))

        new_position = np.array(position)
        new_position[:2] = S

        if all([pi-1e-15 < npi < pi+1e-15 for pi,npi in zip(position, new_position)]):
            raise RecursionError(f'new position {new_position} same as old position {position}')

        return new_position



class Vertex:

    def __init__(self, p, d):
        self.vertex = np.array(p, dtype=np.float64)
        self.diameter = d


    def __repr__(self):
        return repr(self.vertex)


    def __str__(self):
        return str(self.vertex)


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



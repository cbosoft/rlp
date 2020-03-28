
class Particle:

    def __init__(self, position, diameter, id=0):
        self.id = id
        self.position = position
        self.diameter = diameter
        self.settled = False

    def settle_to(self, position=None, position_z=None):

        if position is not None:
            self.position = position
        elif position_z is not None:
            self.position[2] = position_z
        else:
            raise Exception("position or position_z must be specified.")

        self.settled = True

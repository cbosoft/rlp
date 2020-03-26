import numpy as np
from matplotlib import pyplot as plt

from particle import Particle
from sim import Sim

if __name__ == "__main__":
    sim = Sim(10.0)
    for i in range(50):
        sim.generate_particle()
    # TODO test triangle intersection and triangle get

    triangles = sim.find_triangles(2)

    ax = plt.axes([0, 0, 1, 1], projection='3d')

    u, v = np.mgrid[0:2*np.pi:20j, 0:np.pi:10j]
    X = np.cos(u)*np.sin(v)*0.5
    Y = np.sin(u)*np.sin(v)*0.5
    Z = np.cos(v)*0.5

    for particle in sim.particles:
        #plt.gca().add_patch(plt.Circle(particle.position[:2], 0.5))

        #x, y, z = np.add(particle.position, [X, Y, Z] )
        x = np.add(X, particle.position[0])
        y = np.add(Y, particle.position[1])
        z = np.add(Z, particle.position[2])
        ax.plot_surface(x, y, z, color="r")

    for triangle in triangles:
        x, y, z = zip(*triangle.vertices())
        ax.plot(x, y, z, '--', color='0.5')
    ax.set_xlim([0,10])
    ax.set_ylim([0,10])
    ax.set_zlim([0,10])
    plt.show()


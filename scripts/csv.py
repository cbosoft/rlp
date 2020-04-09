from particle import Particle

def read_csv(path):
    with open(path) as csvf:
        lines = csvf.readlines()

    header_data = [v for v in lines[0].split(',')]
    L = float(header_data[0])
    vf = float(header_data[1])
    ff = float(header_data[2])
    if header_data[-1].strip() == 'error':
        print('Warning: sim ended in error.')
    particles = list()
    for line in lines[1:]:
        data = (float(v) for v in line.split(','))
        particles.append(Particle(*data, len(particles)))
    return L, vf, ff, particles

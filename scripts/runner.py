from itertools import product
import subprocess as sp



number = [300]
length = [10]
seed = list(range(10))

monitor = -1
processes = list()
for i, (n, l, s) in enumerate(product(number, length, seed)):
    output_path = f'data/rlp_{n}_{l}_{s}.csv'

    command = f'./rlp --number {n} --length {l} --seed {s} --output-path {output_path}'
    if i != monitor:
        command += ' &> /dev/null'
    print(i, 'STARTED')

    process = sp.Popen(command, shell=True)
    processes.append(process)


for i, process in enumerate(processes):
    rc = process.wait()
    if rc:
        print(i, 'FAILED :', process.stderr.read().decode())
    else:
        print(i, 'FINISHED')


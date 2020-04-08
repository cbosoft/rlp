from itertools import product
import subprocess as sp



number = [300]
length = [10]
seed = list(range(10))
dist = ['mono', 'bi']

nthreads = 3
processes = list()

done = 0
for run_id, (n, l, d, s) in enumerate(product(number, length, dist, seed)):
    output_path = f'data/rlp_{n}_{l}_{d}_{s}.csv'

    command = f'./rlp --number {n} --length {l} --seed {s} --output-path {output_path} --disperse {d}'
    command += ' &> /dev/null'

    if len(processes) >= nthreads:
        waiting = True
        while waiting:
            for i, (run_id_f, process) in enumerate(processes):
                rc = process.poll()
                if rc is None:
                    continue
                else:
                    if rc:
                        print(run_id_f, 'FAILED :', process.stderr.read().decode())
                    else:
                        print(run_id_f, 'FINISHED')
                    done += 1
                    processes.pop(i)
                    waiting = False
                    break

    process = sp.Popen(command, shell=True)
    processes.append([run_id, process])
    print(run_id, 'STARTED')


for run_id, process in processes:
    rc = process.wait()
    if rc:
        print(run_id, 'FAILED :', process.stderr.read().decode())
    else:
        print(run_id, 'FINISHED')


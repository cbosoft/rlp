from itertools import product
import subprocess as sp

from pb import ProgressBar, FG_GREEN, FG_RED, RESET


number = [200]
length = [10]
seed = list(range(10))
dist = ['mono', 'bi']

nthreads = 3
processes = list()
monitored = None

parameter_combinations = list(product(number, length, dist, seed))

pb = ProgressBar(len(parameter_combinations))
for run_id, (n, l, d, s) in enumerate(parameter_combinations):
    output_path = f'data/rlp_{n}_{l}_{d}_{s}.csv'

    command = f'./rlp -q --number {n} --length {l} --seed {s} --output-path {output_path} --disperse {d}'

    if len(processes) >= nthreads:
        waiting = True
        while waiting:
            for i, (run_id_f, process) in enumerate(processes):
                so, se = process.communicate()
                rc = process.poll()
                if rc is None:
                    continue
                else:
                    status = FG_RED+'FAILED!'+RESET if rc else FG_GREEN+'FINISHED'+RESET
                    pb.print(run_id, status)
                    pb.update(rc == 0)
                    processes.pop(i)
                    waiting = False
                    break

    process = sp.Popen(command, shell=True, stdout=sp.PIPE, stderr=sp.PIPE)
    processes.append([run_id, process])
    pb.print(run_id, 'STARTED')


for run_id, process in processes:
    process.communicate()
    rc = process.wait()
    status = FG_RED+'FAILED!'+RESET if rc else FG_GREEN+'FINISHED'+RESET
    pb.print(run_id, status)
    pb.update(rc == 0)


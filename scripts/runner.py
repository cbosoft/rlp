from itertools import product
import subprocess as sp



number = [1000]
length = [10]
seed = list(range(10))
dist = ['mono', 'bi']

nthreads = 10
processes = list()
monitored = None

done = 0
for run_id, (n, l, d, s) in enumerate(product(number, length, dist, seed)):
    output_path = f'data/rlp_{n}_{l}_{d}_{s}.csv'

    command = f'./rlp -q --number {n} --length {l} --seed {s} --output-path {output_path} --disperse {d}'

    if len(processes) >= nthreads:
        waiting = True
        while waiting:
            for i, (run_id_f, process) in enumerate(processes):
                process.communicate()
                rc = process.poll()
                if rc is None:
                    continue
                else:
                    if rc:
                        print(run_id_f, 'FAILED!')
                    else:
                        print(run_id_f, 'FINISHED')
                    done += 1
                    processes.pop(i)
                    waiting = False
                    if monitored == process:
                        monitored = None
                    break

    if monitored is None:
        process = sp.Popen(command, shell=True)
        monitored = process
    else:
        process = sp.Popen(command, shell=True, stdout=sp.PIPE, stderr=sp.PIPE)
    processes.append([run_id, process])
    print(run_id, 'STARTED')


for run_id, process in processes:
    process.communicate()
    rc = process.wait()
    if rc:
        print(run_id, 'FAILED!')
    else:
        print(run_id, 'FINISHED')


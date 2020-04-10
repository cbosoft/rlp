from itertools import product
import subprocess as sp
import json

from pb import ProgressBar, FG_GREEN, FG_RED, RESET

def update_progress(data, pb):
    total_in_progress = 0
    total_success = 0
    total_failed = 0
    for key, proc_data in data.items():
        total_in_progress += proc_data['in_progress']
        total_success += proc_data['complete']
        total_failed += proc_data['failed']
    pb.set_values(total_in_progress, total_success, total_failed)

def check_on_process(run_id, process, data, pb):
    update_progress(data, pb)
    try:
        inaction, error, settled = json.loads(process.stdout.readline().decode())
        data[run_id]['in_progress'] = settled
    except:
        settled = data[run_id]['in_progress']

    rc = process.poll()
    if rc is None:
        return True
    else:
        data[run_id]['in_progress'] = 0
        if rc:
            data[run_id]['failed'] = settled
        else:
            data[run_id]['complete'] = settled
        return False

if __name__ == "__main__":
    pr = sp.Popen('make rlp', shell=True, stdout=sp.PIPE, stderr=sp.PIPE)
    if pr.wait():
        print(pr.stderr.read().decode())
        exit(1)
    
    
    number = [8400]
    length = [20]
    seed = list(range(10))
    dist = ['mono']
    
    nthreads = 3
    processes = list()
    monitored = None
    
    parameter_combinations = list(product(number, length, dist, seed))
    
    total_number = 0
    for n, __, __, __ in parameter_combinations:
        total_number += n
    
    pb = ProgressBar(total_number)
    data = dict()
    for run_id, (n, l, d, s) in enumerate(parameter_combinations):
        output_path = f'data/rlp_{n}_{l}_{d}_{s}.csv'
    
        command = f'./rlp --progress-json --number {n} --length {l} --seed {s} --output-path {output_path} --sieve {d} --error-tolerance most'
    
        if len(processes) >= nthreads:
            waiting = True
            while waiting:
                for i, (run_id_f, process) in enumerate(processes):
                    if not check_on_process(run_id_f, process, data, pb):
                        processes.pop(i)
                        waiting = False
                        break
    
        process = sp.Popen(command, shell=True, stdout=sp.PIPE, stderr=sp.PIPE)
        processes.append([run_id, process])
        data[run_id] = {'in_progress':0, 'complete':0, 'failed':0}
    
    
    for run_id, process in processes:
        check_on_process(run_id, process, data, pb)

    print()

import os
from time import sleep
import random

FG_GREEN   = u"\u001b[32m"
FG_RED     = u"\u001b[31m"
RESET      = u"\u001b[0m"

class ProgressBar:

    def __init__(self, length):
        self.length = length
        self.success = 0
        self.neutral = 0
        self.fail = 0

    def update(self, success):

        if success:
            self.success += 1
        else:
            self.fail += 1

        self.draw()

    def set_values(self, neutral, success, fail):
        self.neutral = neutral
        self.success = success
        self.fail = fail
        self.draw()


    def draw(self):
        rows, columns = os.popen('stty size', 'r').read().split()
        frac = f' {self.success}/{self.length} '
        columns = int(columns) - len(frac)
        col_per_i = float(columns) / float(self.length)

        neutral_prog = int(col_per_i * self.neutral)
        success_prog = int(col_per_i * self.success)
        fail_prog = int(col_per_i * self.fail)
        un_prog = columns - neutral_prog - success_prog - fail_prog

        neutral_prog = '█'*neutral_prog
        success_prog = FG_GREEN + '█'*success_prog + RESET
        fail_prog = FG_RED + '█'*fail_prog + RESET
        un_prog = ' '*un_prog

        end = '' if (self.success+self.fail+self.neutral) < (self.length) else '\n'
        print(f'{frac}{neutral_prog}{success_prog}{fail_prog}{un_prog}\r', end=end)

    def clear(self):
        print('\u001b[2K', end='')

    def print(self, *args, **kwargs):
        self.clear()
        print(*args, **kwargs)
        self.draw()

if __name__ == "__main__":

    n = 20
    pb = ProgressBar(n)
    
    for i in range(n):
        pb.update(random.randint(0,100) <= 80)
        sleep(0.5)

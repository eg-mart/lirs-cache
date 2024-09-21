import sys
import subprocess
from subprocess import STDOUT, PIPE
from random import randint
import matplotlib.pyplot as plt

program1 = 'build/ideal'
program2 = 'build/lirs'

hits_diff = {}
prog2_res = []
prog1_res = []

NUM_TESTS = 100
MAX_NUM_REQUESTS = 10000

for test_id in range(NUM_TESTS):
    num_requests = randint(1, MAX_NUM_REQUESTS)
    cache_size = max(2, int(0.1 * num_requests))

    test = f'{cache_size}\n{num_requests}\n'
    for i in range(num_requests):
        test += str(randint(1, cache_size * 2)) + '\n'

    res1 = subprocess.run(program1, encoding='ascii', input=test, stdout=PIPE,
                          stderr=STDOUT, text=True)

    if res1.returncode != 0:
        print(f"Error running {program1}:")
        print(res1.stdout)

    res2 = subprocess.run(program2, encoding='ascii', input=test, stdout=PIPE, 
                          stderr=STDOUT, text=True)

    if res2.returncode != 0:
        print(f"Error running {program2}:")
        print(res2.stdout)

    hits_diff[cache_size] = (int(res1.stdout) - int(res2.stdout))
    prog2_res.append(int(res2.stdout))
    prog1_res.append(int(res1.stdout))

hits_sorted = sorted(hits_diff.items(), key=lambda x: x[0])
plt.plot([i for i in range(NUM_TESTS)], sorted(prog1_res), label=program1)
plt.plot([i for i in range(NUM_TESTS)], sorted(prog2_res), label=program2)
plt.legend()
plt.savefig(f"benchmark.png")

import sys
import subprocess
from subprocess import STDOUT, PIPE
from random import randint
import matplotlib.pyplot as plt

program1 = 'build/ideal'
program2 = 'build/lirs'

hits_diff = {}

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

hits_sorted = sorted(hits_diff.items(), key=lambda x: x[0])
plt.plot([e[0] for e in hits_sorted], [e[1] for e in hits_sorted])
plt.savefig(f"benchmark.png")

import sys, time, subprocess, random, string
from sympy.combinatorics import Permutation

Permutation.print_cyclic = False

total = 5
success = 0
maxlen = 10
maxpower = 100000

for i in range(total):
	len = random.randrange(1, maxlen)
	power = random.randrange(-maxpower, maxpower)
	perm = Permutation.random(len)


	proc = subprocess.Popen(['./stress', str(len)] + list(map(str, perm.array_form)), stdout=subprocess.PIPE)
	c = proc.stdout.readline()
	result = int(c)
	#result = Permutation(list(map(lambda x: int(x) - 1, str.split(str(c)[3:-2]))))

	if (result == 1) == perm.is_even:
		success = success + 1
	else:
		print("Tried " + str(perm))
		print("Expected " + str(perm.is_even))
		print("Got " + str(result));
		print()


print(str(success) + "/" + str(total));

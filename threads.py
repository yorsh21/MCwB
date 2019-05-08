import os
import sys
import time
import subprocess

inputs = ["a36", "a62", "a64", "a65", "eil22", "eil51", "eil76", "tai75A"]
seeds = [1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 153935025]
#seeds = [1539344818, 1539344696, 1539344634, 1539344526, 1539344434, 1539344472, 1539343457, 1539342487, 1539342076, 1539340235]
times = [110, 1022, 5395, 478, 12, 154, 1700, 4806]
qualities = [29233, 22917, 24100, 28046, 15947, 50128, 91461, 65477]



if len(sys.argv) == 1:
	os.system("make")

	for i in range(len(inputs)):
		for seed in seeds:
			subprocess.Popen(['./main', inputs[i], str(seed), str(times[i]), str(qualities[i])])
		time.sleep(times[i])

elif len(sys.argv) == 2:
	i = inputs.index(sys.argv[1])

	os.system("make")

	for seed in seeds:
		subprocess.Popen(['./main', inputs[i], str(seed), str(times[i]), str(qualities[i])])

elif len(sys.argv) == 3:
	i = inputs.index(sys.argv[1])
	seed = sys.argv[2];

	os.system("make")

	os.system('./main' + " " + inputs[i] + " " + str(seed) + " " + str(times[i]) + " " + str(qualities[i]))

else:
	print("Excess of parameters")






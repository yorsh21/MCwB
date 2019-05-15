import os
import sys
import time
import subprocess

inputs = ["a36", "a62", "a64", "a65", "eil22", "eil51", "eil76", "tai75A"]
seeds = [1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 153935025]
#seeds = [1539344818, 1539344696, 1539344634, 1539344526, 1539344434, 1539344472, 1539343457, 1539342487, 1539342076, 153934023]
#seeds = [1539314718, 1539144796, 1539144734, 1539344726, 1539344731, 1539341772, 1139343757, 1539342787, 1539342716, 153931075]
times = [110, 1022, 5395, 478, 12, 154, 1700, 4806]
qualities = [29233, 22917, 24100, 28046, 15947, 50128, 91461, 65477]

os.system("make")
process = []

if len(sys.argv) == 1:
	for i in range(len(inputs)):
		for seed in seeds:
			process.append(subprocess.Popen(['./main', inputs[i], str(seed), str(times[i]), str(qualities[i])]))

		time.sleep(times[i]+60)

		for p in process:
			p.terminate()

		process = []

elif len(sys.argv) == 2:
	i = inputs.index(sys.argv[1])

	for seed in seeds:
		process.append(subprocess.Popen(['./main', inputs[i], str(seed), str(times[i]), str(qualities[i])]))

	time.sleep(times[i]+60)

	for p in process:
		p.terminate()

elif len(sys.argv) == 3:
	i = inputs.index(sys.argv[1])
	seed = sys.argv[2];

	os.system('./main' + " " + inputs[i] + " " + str(seed) + " " + str(times[i]) + " " + str(qualities[i]))

else:
	print("Excess of parameters")






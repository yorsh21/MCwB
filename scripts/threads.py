import os
import sys
import time
import subprocess

inputs = ["a33", "a34", "a36", "a37", "a38", "a39", "a44", "a45", "a46", "a48", "a53", "a54", "a55", "a60", "a61", "a62", "a63", "a64", "a65", "a69", "a80", "c50", "c75", "tai75A", "tai75B", "tai75C", "tai75D", "f45", "f71", "f72", "eil22", "eil23", "eil30", "eil33", "eil51", "eil76", "att48"]
seeds = [1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 153935025]
#seeds = [1539344818, 1539344696, 1539344634, 1539344526, 1539344434, 1539344472, 1539343457, 1539342487, 1539342076, 153934023]
#seeds = [1539314718, 1539144796, 1539144734, 1539344726, 1539344731, 1539341772, 1139343757, 1539342787, 1539342716, 153931075]
times = [23, 62, 40, 110, 45, 570, 110, 101, 136, 66, 230, 183, 304, 270, 3565, 561, 1022, 2930, 5395, 478, 1552, 5626, 84, 13760, 4806, 15056, 4537, 2645, 80, 3483, 3659, 12, 6, 99, 66, 58, 154, 1700, 66843, 284]
qualities = [26660, 29417, 30496, 29233, 24837, 28596, 30808, 38771, 40282, 40696, 39800, 46662, 22414, 24694, 25041, 60644, 22917, 24447, 24100, 28046, 25822, 29977, 49803, 86677, 65477, 48238, 25906, 65477, 23705, 72864, 72072, 15947, 7207, 7117, 60080, 20409, 50128, 91461, 96115, 17452]

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






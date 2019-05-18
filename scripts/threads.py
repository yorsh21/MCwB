import os
import sys
import time
import subprocess

seeds = [1437356881, 1437356667, 1437356643, 1437356562, 1437356443, 1437356427, 1437356575, 1437356478, 1437356067, 143735625]
#seeds = [1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 153935025]
instances = [
	["a33", 62, 29417],
	["a34", 40, 30496],
	["a36", 110, 29233],
	["a37", 45, 24837],
	["a38", 570, 28596],
	["a39", 110, 30808],
	["a44", 101, 38771],
	["a45", 136, 40282],
	["a46", 66, 40696],
	["a48", 230, 39800],
	["a53", 183, 46662],
	["a54", 304, 22414],
	["a55", 270, 24694],
	["a60", 3565, 25041],
	["a61", 561, 60644],
	["a62", 1022, 22917],
	["a63", 2930, 24447],
	["a64", 5395, 24100],
	["a65", 478, 28046],
	["a69", 1552, 25822],
	["a80", 5626, 29977],
	["c50", 84, 49803],
	["c75", 13760, 86677],
	["tai75A", 4806, 65477],
	["tai75B", 15056, 48238],
	["tai75C", 4537, 25906],
	["tai75D", 2645, 65477],
	["f45", 80, 23705],
	["f71", 3483, 72864],
	["f72", 3659, 72072],
	["eil22", 12, 15947],
	["eil23", 6, 7207],
	["eil30", 99, 7117],
	["eil33", 58, 20409],
	["eil51", 154, 50128],
	["eil76", 1700, 91461],
	["att48", 284, 17452]
]

os.system("make")
process = []

if len(sys.argv) == 1:
	for index in range(len(instances)):
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2])]))

		time.sleep(instances[index][1]+10)

		for p in process:
			p.terminate()

		process = []

elif len(sys.argv) == 2:
	index = -1

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2])]))

		time.sleep(instances[index][1]+10)

		for p in process:
			p.terminate()

elif len(sys.argv) == 3:
	index = -1
	seed = sys.argv[2];

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		os.system('./main' + " " + instances[index][0] + " " + str(seed) + " " + str(instances[index][1]) + " " + str(instances[index][2]))

else:
	print("Excess of parameters")





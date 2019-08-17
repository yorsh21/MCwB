#!/usr/bin/env python3

import os
import sys
import time
import subprocess

seeds = [
	1437356881, 1437356667, 1437356643, 1437356562, 1437356443, 1437356427, 1437356575, 1437356478, 1437356067, 143735625,
	1192356381, 1192356367, 1192356343, 1192356362, 1192356343, 1192356327, 1192356375, 1192356378, 1192356367, 119235635,
	1283156388, 1283156047, 1283156327, 1283156012, 1283156343, 1283150322, 1280156375, 1283106375, 1283156364, 128315039
]

instances = [
	["a33", 62, 29417, 300],
	["a34", 40, 30496, 300],
	["a36", 110, 29233, 300],
	["a37", 45, 24837, 300],
	["a38", 570, 28596, 300],
	["a39", 110, 30808, 300],
	["a44", 101, 38771, 300],
	["a45", 136, 40282, 300],
	["a46", 66, 40696, 300],
	["a48", 230, 39800, 300],
	["a53", 183, 46662, 300],
	["a54", 304, 22414, 300],
	["a55", 270, 24694, 300],
	["a60", 3565, 25041, 300],
	["a61", 561, 60644, 300],
	["a62", 1022, 22917, 300],
	["a63", 2930, 24447, 300],
	["a64", 5395, 24100, 300],
	["a65", 478, 28046, 300],
	["a69", 1552, 25822, 300],
	["a80", 5626, 29977, 300],
	["c50", 84, 66160, 300],
	["c75", 13760, 46180, 300],
	["tai75A", 4806, 42992, 300],
	["tai75B", 15056, 48238, 300],
	["tai75C", 4537, 25906, 300],
	["tai75D", 2645, 43869, 300],
	["f45", 80, 23705, 300],
	["f71", 3483, 72864, 300],
	["f72", 3659, 72072, 300],
	["eil22", 12, 15947, 300],
	["eil23", 6, 7207, 300],
	["eil30", 99, 7117, 300],
	["eil33", 58, 20409, 300],
	["eil51", 154, 50128, 300],
	["eil76", 1700, 91461, 300],
	["att48", 284, 17452, 300],
	["_real", 5959, 13173, 600]
]

os.system("make")

factor1 = "10"
factor2 = "10"
step = 10
process = []

#Run all instances with all threads
if len(sys.argv) == 1:
	total_entries = 0

	#Excluye la instancia REAL
	for index in range(len(instances) - 1):
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2]), factor1, factor2]))

		steps = 0
		if(os.path.isfile(os.path.dirname(__file__) + "/../outputs/results_threads.out")):
			entries = len(open(os.path.dirname(__file__) + "/../outputs/results_threads.out").readlines())
		else:
			entries = 0

		while(entries < total_entries + len(seeds) and steps < instances[index][1]):
			time.sleep(step)
			steps += step

			entries = len(open(os.path.dirname(__file__) + "/../outputs/results_threads.out").readlines())
			if(entries % 60 == 0):
				print(entries)
		
		time.sleep(instances[index][3])

		for p in process:
			p.terminate()

		process = []
		total_entries = len(open(os.path.dirname(__file__) + "/../outputs/results_threads.out").readlines())

#Run an instance with all threads
elif len(sys.argv) == 2:
	index = -1

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2]), factor1, factor2]))

		time.sleep(instances[index][1]+instances[index][3])

		for p in process:
			p.terminate()

#Run an instance with one threads
elif len(sys.argv) == 3:
	index = -1
	seed = sys.argv[2];

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		os.system('./main' + " " + instances[index][0] + " " + str(seed) + " " + str(instances[index][1]) + " " + str(instances[index][2]) + " " + factor1 + " " + factor2)

else:
	print("Excess of parameters")






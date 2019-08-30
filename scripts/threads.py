#!/usr/bin/env python3

import os
import sys
import time
import subprocess

factor1 = "10"
factor2 = "10"
disturbing = "0.05"

seeds = [
	1437356881, 1437356667, 1437356643, 1437356562, 1437356443, 1437356427, 1437356575, 1437356478, 1437356067, 143735625,
	1192356381, 1192356367, 1192356343, 1192356362, 1192356343, 1192356327, 1192356375, 1192356378, 1192356367, 119235635,
	1283156388, 1283156047, 1283156327, 1283156012, 1283156343, 1283150322, 1280156375, 1283106375, 1283156364, 128315039
]

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
	["c50", 84, 66160],
	["c75", 13760, 46180],
	["tai75A", 4806, 42992],
	["tai75B", 15056, 48238],
	["tai75C", 4537, 25906],
	["tai75D", 2645, 43869],
	["f45", 80, 23705],
	["f71", 3483, 72864],
	["f72", 3659, 72072],
	["eil22", 12, 15947],
	["eil23", 6, 7207],
	["eil30", 99, 7117],
	["eil33", 58, 20409],
	["eil51", 154, 50128],
	["eil76", 1700, 91461],
	["att48", 284, 17452],
	["_real", 5959, 14155] #13173]
]


os.system("make")

process = []

#Run all instances with all threads
if len(sys.argv) == 1:

	for index in range(len(instances)):
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]*0+3600*15), str(instances[index][2]), factor1, factor2, disturbing]))

		for p in process:
			p.communicate()

		process = []


#Run an instance with all threads
elif len(sys.argv) == 2:
	index = -1

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]*0+3600*15), str(instances[index][2]), factor1, factor2, disturbing]))

		for p in process:
			p.communicate()


#Run an instance with one threads
elif len(sys.argv) == 3:
	index = -1
	seed = sys.argv[2];

	for i in range(len(instances)):
		if instances[i][0] == sys.argv[1]:
			index = i
			break

	if index != -1:
		os.system('./main' + " " + instances[index][0] + " " + str(seed) + " " + str(instances[index][1]*0+3600*15) + " " + str(instances[index][2]) + " " + factor1 + " " + factor2 + " " + disturbing)

else:
	print("Excess of parameters")






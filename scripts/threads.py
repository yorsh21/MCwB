#!/usr/bin/env python3

import os
import sys
import time
import subprocess
import psutil

factor1 = "10"
factor2 = "10"
disturbing = "0.05"

seeds = [
	1437356881, 1437356667, 1437356643, 1437356562, 1437356443, 1437356427, 1437356575, 1437356478, 1437356067, 143735625,
	1192356381, 1112356364, 1192356343, 1192356362, 1102834712, 1192356327, 1192356375, 1192356378, 1192356367, 119235635,
	1283156388, 1283156047, 1283156327, 1283156012, 1283156343, 1283150322, 1280156375, 1283106375, 1283156364, 128315039,
	#1196254781, 1295352671, 1112356349, 1152356362, 1113356443, 1592456323, 1332352330, 1621356679, 1052336261, 178635136,
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
	#["a60", 3565, 25041],
	["a60", 3565, 24587],
	["a61", 561, 60644],
	["a62", 1022, 22917],
	["a63", 2930, 24447],
	["a64", 5395, 24100],
	["a65", 478, 28046],
	["a69", 1552, 25822],
	["a80", 5626, 29977],
	#["c50", 84, 66160],
	["c50", 84, 65477],
	#["c75", 13760, 46180],
	["c75", 13760/2, 42423],
	#["tai75A", 4806, 42992],
	["tai75A", 4806, 40505],
	["tai75B", 15056/3, 48238],
	["tai75C", 4537, 25906],
	#["tai75D", 2645, 43869],
	["tai75D", 2645, 42484],
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
	#["_real", 5959, 14155]
	#["_real", 57962, 14155]
]

def len_process(name):
	counter = 0
	for proc in psutil.process_iter():
	    try:
	    	if proc.name() == name:
	    		counter += 1
	    except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
	        pass
	return counter

def if_process(name):
	result = subprocess.run(['ps', '-e'], stdout=subprocess.PIPE)
	output = result.stdout.decode('utf-8')
	bolean = name in output
	return name in output

os.system("make")

process = []

#Run all instances with all threads
if len(sys.argv) == 1:

	for index in range(len(instances)):
		for seed in seeds:
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2]), factor1, factor2, disturbing]))

		while not if_process("main <defunct>"):
			time.sleep(5)

		for p in process:
			p.terminate()
			p.communicate()

		while if_process("main"):
			time.sleep(5)

		#for p in process:
		#	p.communicate()

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
			process.append(subprocess.Popen(['./main', instances[index][0], str(seed), str(instances[index][1]), str(instances[index][2]), factor1, factor2, disturbing]))

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
		os.system('./main' + " " + instances[index][0] + " " + str(seed) + " " + str(instances[index][1]) + " " + str(instances[index][2]) + " " + factor1 + " " + factor2 + " " + disturbing)

else:
	print("Excess of parameters")






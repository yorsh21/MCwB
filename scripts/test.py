#!/usr/bin/env python3

file = open("test").read().split("\n")

temp = [0, 0, 0, 0, -999999, -999999, -999999]
counter = 0
factor = 0

for line in file:
	arr = line.split("\t")
	counter += 1

	temp[1] = temp[0] + int(arr[0])
	temp[2] = temp[1] + int(arr[1])
	temp[3] = temp[2] + int(arr[2])

	temp[4] = max(temp[4], int(arr[0]))
	temp[5] = max(temp[5], int(arr[1]))
	temp[6] = max(temp[6], int(arr[2]))

	if(counter % 30 == 0):
		temp[1] = int(int(temp[1])/30)
		temp[2] = int(int(temp[2])/30)
		temp[3] = int(int(temp[3])/30)

		factor += 2
		temp[0] = factor

		print('\t'.join(str(e) for e in temp))

		temp = [0, 0, 0, 0, -999999, -999999, -999999]


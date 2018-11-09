#!/usr/bin/env python
# coding=utf-8

import sys
import numpy as np
from pylab import *
import matplotlib.pyplot as plt

def read_instances(instance):
	file = open("inputs/" + instance)

	num_trucks = int(file.readline())
	trucks_capacities = file.readline()[:-1].split("\t")
	file.readline()
	num_milks = int(file.readline())
	milk_types = file.readline()[:-1].split("\t")
	milk_values = file.readline()[:-1].split("\t")
	file.readline()
	num_farms = int(file.readline())

	farms = list()
	for farm in range(num_farms):
	    farms.append(file.readline()[:-1].split("\t"))

	return farms


def plot_map(instance, route = None):
	colors = ['c', 'm', 'y']
	line = '-'
	plt.figure(num=None, figsize=(14, 10), dpi=100, facecolor='w', edgecolor='k')
	plt.title('Draw Milk Routes')
	plt.xlabel('x')
	plt.ylabel('y')

	
	for i in range(len(farms)-1):
	    if farms[i][3] == "-":
	        scatter(int(farms[i][1]), int(farms[i][2]), s=80 ,marker='s', c='k')
	    elif farms[i][3] == "A":
	        scatter(int(farms[i][1]), int(farms[i][2]), s=30 ,marker='o', c='r')
	    elif farms[i][3] == "B":
	        scatter(int(farms[i][1]), int(farms[i][2]), s=30 ,marker='o', c='g')
	    elif farms[i][3] == "C":
	        scatter(int(farms[i][1]),int(farms[i][2]), s=30 ,marker='o', c='b')
	        
	if route is not None:
		count_route = -1
		for i in range(len(route)-1):
			if route[i] == 0:
				count_route += 1

			plot([int(farms[route[i]][1]), int(farms[route[i+1]][1])],
				[int(farms[route[i]][2]), int(farms[route[i+1]][2])], colors[count_route]+line, linewidth = .5)

	#plt.grid()
	#plt.show()
	plt.savefig("outputs/" + instance.replace("txt", "png"))



#Read Args
if len(sys.argv) == 1:
	print("It is need pass the instance name")
elif len(sys.argv) == 2:
	farms = read_instances(sys.argv[1])
	plot_map()
elif len(sys.argv) == 3:
	farms = read_instances(sys.argv[1])
	route = list(map(int, sys.argv[2][1:-1].split(",")))
	plot_map(sys.argv[1], route)


#route = [0, 34, 15, 26, 24, 7, 2, 0, 8, 21, 6, 30, 33, 20, 5, 17, 31, 12, 35, 1, 16, 4, 0, 13, 32, 19, 27, 3, 22, 29, 23, 14, 11, 28, 18, 10, 25, 9, 0]

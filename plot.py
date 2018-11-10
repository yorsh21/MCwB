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
	plt.title('Route: ' + sys.argv[2])
	plt.xlabel('x')
	plt.ylabel('y')

	
	for f in farms:
	    if f[3] == "-":
	        scatter(int(f[1]), int(f[2]), s=80 ,marker='s', c='k')
	    elif f[3] == "A":
	        scatter(int(f[1]), int(f[2]), s=30 ,marker='o', c='r')
	    elif f[3] == "B":
	        scatter(int(f[1]), int(f[2]), s=30 ,marker='o', c='g')
	    elif f[3] == "C":
	        scatter(int(f[1]),int(f[2]), s=30 ,marker='o', c='b')
	    plt.text(int(f[1]), int(f[2]), int(f[0])-1, fontsize=9)
	        
	if route is not None:
		count_route = -1
		routex = [int(farms[int(route[0])][1])]
		routey = [int(farms[int(route[0])][2])]

		for i in range(1, len(route)):
			routex.append(int(farms[int(route[i])][1]))
			routey.append(int(farms[int(route[i])][2]))

			if int(route[i]) == 0:
				plot(routex, routey, colors[count_route]+line, linewidth = .5)
				count_route += 1
				routex = [int(farms[int(route[i])][1])]
				routey = [int(farms[int(route[i])][2])]


			#plot([int(farms[int(route[i])-1][1]), int(farms[int(route[i+1])-1][1])],
			#	[int(farms[int(route[i])-1][2]), int(farms[int(route[i+1])-1][2])], colors[count_route]+line, linewidth = .5)

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

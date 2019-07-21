#!/usr/bin/env python3
# coding=utf-8

import sys
import os.path
from pylab import *

num_trucks = 0
trucks_capacities = []
num_milks = 0
milk_request = []
milk_values = []
num_farms = 0
farms = []
letters = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"];


def convert_array(element):
	try:
	   return int(element)
	except ValueError:
		if(element in letters):
			return letters.index(element)
		else:
			return -1


def read_instances(instance):
	file = open(os.path.dirname(__file__) + "/../inputs/" + instance + ".txt")

	global num_trucks
	global trucks_capacities
	global num_milks
	global milk_request
	global milk_values
	global num_farms

	num_trucks = int(file.readline())
	trucks_capacities = file.readline()[:-1].split("   ")
	file.readline()

	num_milks = int(file.readline())
	milk_request = file.readline()[:-1].split("    ")
	milk_values = file.readline()[:-1].split("     ")
	file.readline()

	num_farms = int(file.readline())

	for farm in range(num_farms):
		line = file.readline()[:-1]
		array = list(map(convert_array, line.split("\t")))
		farms.append(array)


def total_cost(route):
	route_cost = []
	temp_cost = 0
	
	for i in range(1, len(route)):
		if route[i] == 0:
			
			temp_cost += int(
				sqrt(
					(farms[route[i-1]][1] - farms[route[i]][1])**2 + 
					(farms[route[i-1]][2] - farms[route[i]][2])**2
				) + 0.5
			)

			route_cost.append(temp_cost)

			temp_cost = 0
		else:
			temp_cost += int(
				sqrt(
					(farms[route[i-1]][1] - farms[route[i]][1])**2 + 
					(farms[route[i-1]][2] - farms[route[i]][2])**2
				) + 0.5
			)

	return int(sum(route_cost))


def local_cost(route):
	route_cost = 0
	
	for i in range(1, len(route)):
		route_cost += int(
			sqrt(
				(farms[route[i-1]][1] - farms[route[i]][1])**2 + 
				(farms[route[i-1]][2] - farms[route[i]][2])**2
			) + 0.5
		)

	return route_cost

def swap(route, index1, index2):
	temp = route[index2]
	route[index2] = route[index1]
	route[index1] = temp
	return route


def find_better(route):
	global_cost = total_cost(route)
	routes = []
	index = 0

	for i in range(1, len(route)):
		if(route[i] == 0):
			routes.append(route[index:i+1])
			index = i

	
	for route in routes:
		droute = route
		route_cost = local_cost(route)
		print("Route: ", route_cost, route, "\n")

		for g in range(1, len(route)-1):
			for h in range(1, len(route)-1):
				droute = route

				for i in range(1, len(droute)-1):
					for j in range(1, len(droute)-1):
						temp = swap(droute, i, j)
						temp_cost = local_cost(temp)

						if(temp_cost < route_cost):
							route_cost = temp_cost
							print(temp_cost, " -> ", temp)
	

	print("Finish")



#Read Args
if len(sys.argv) != 3:
	print("It is need pass the instance name")
else:
	read_instances(sys.argv[1])

	route = list(map(int, sys.argv[2][1:-1].split(",")))

	print()

	find_better(route)

'''
python3 better.py a64 [0,41,56,32,20,23,62,53,14,17,11,38,29,0,25,22,43,28,1,7,40,49,55,31,37,19,34,4,58,46,16,61,13,52,10,0,6,33,48,45,15,57,30,3,51,9,42,24,60,8,47,63,2,54,5,59,50,36,39,44,21,26,35,27,12,18,0]

python3 better.py a64 [0,41,20,56,32,23,62,53,14,17,11,38,29,0,25,22,43,28,1,7,40,49,55,31,37,19,34,4,58,46,16,61,13,52,10,0,6,33,48,45,15,57,30,3,51,9,42,24,60,8,47,63,2,54,5,59,50,36,39,44,21,26,35,27,12,18,0]
'''
#!/usr/bin/env python
# coding=utf-8

import sys
from pylab import *
import itertools

num_trucks = 0
trucks_capacities = []
num_milks = 0
milk_request = []
milk_values = []
num_farms = 0
farms = []

global_cost = 99999999

def read_instances(instance):
	file = open("inputs/" + instance + ".txt")

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
		line = file.readline()[:-1].replace("A", "0").replace("B", "1").replace("C", "2").replace("-", "-1")
		farms.append(list(map(int, line.split("\t"))))


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


def local_cost_zero(route):
	route_cost = 0

	route_cost += int(
		sqrt(
			(farms[0][1] - farms[route[1]][1])**2 + 
			(farms[0][2] - farms[route[1]][2])**2
		) + 0.5
	)
	
	for i in range(1, len(route)):
		route_cost += int(
			sqrt(
				(farms[route[i-1]][1] - farms[route[i]][1])**2 + 
				(farms[route[i-1]][2] - farms[route[i]][2])**2
			) + 0.5
		)

	route_cost += int(
		sqrt(
			(farms[route[-1]][1] - farms[0][1])**2 + 
			(farms[route[-1]][2] - farms[0][2])**2
		) + 0.5
	)

	return route_cost


def swap(route, index1, index2):
	temp = route[index2]
	route[index2] = route[index1]
	route[index1] = temp
	return route


def find_better(route):
	global global_cost

	routes = []
	index = 0
	tupla = (0,)

	for i in range(1, len(route)):
		if(route[i] == 0):
			routes.append(route[index:i+1])
			index = i

	
	r = routes[0][1:-1]
	global_cost = local_cost(r)

	options = itertools.permutations(routes[0], len(routes[0]))
	for o in options:
	    #if local_cost_zero(o) != local_cost(tupla + o + tupla):
	    #	print(str(local_cost_zero(o)) + " - " + str(local_cost(tupla + o + tupla)))
	    #if local_cost(tupla + o + tupla) < global_cost:
	    #	global_cost = local_cost(tupla + o + tupla)
	    #	print(global_cost)
	    #	print(o)
	    print(o)

	print("Finish")



#Read Args
if len(sys.argv) != 3:
	print("It is need pass the instance name")
else:
	read_instances(sys.argv[1])

	route = list(map(int, sys.argv[2][1:-1].split(",")))

	find_better(route)

#python3 better.py a36 [0,10,7,34,28,4,19,31,13,22,25,1,16,0,11,32,17,29,14,23,2,35,20,5,26,0,27,24,21,18,30,33,12,3,6,9,8,15,0]

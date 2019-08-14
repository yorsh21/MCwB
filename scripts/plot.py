#!/usr/bin/env python3
# coding=utf-8

import math
import sys
import numpy as np
from pylab import *
from numpy.linalg import eig
import matplotlib.pyplot as plt
import os.path

num_trucks = 0
trucks_capacities = []
num_milks = 0
milk_request = []
milk_values = []
num_farms = 0
coordenates = []
cost_matrix = []
types_farms = []
milk_farms = []
milk_by_route = []
letters = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"];

def int_convert_array(element):
	try:
	   return int(float(element) + 0.5)
	except ValueError:
		return -1

def float_convert_array(element):
	try:
	   return float(element)
	except ValueError:
		if(element in letters):
			return letters.index(element)
		else:
			return -1

def get_coordenates(cost_matrix):
	cost_square = [row[:] for row in cost_matrix]
	for i in range(len(cost_square)):
		for j in range(len(cost_square)):
			if i < j:
				cost_square[i][j] = cost_square[i][j]**2
			else:
				cost_square[i][j] = cost_square[j][i]

	M = []
	for i in range(len(cost_square)):
		Mi = []
		for j in range(len(cost_square)):
			Mij = cost_square[0][j] + cost_square[i][0] - cost_square[i][j]
			Mi.append(Mij/2)
		M.append(Mi)

	npM = np.array(M)
	values, vectors = eig(npM)

	coordenates = []
	for i in range(len(values)):
		if values[i].real > 0.001:
			coord = sqrt(values[i].real)*vectors.T[i]
			coordenates.append(coord.tolist())

	list_coords = []
	for i in range(len(coordenates[0])):
		list_coords.append([coordenates[0][i].real, coordenates[1][i].real])


	##################################
	'''
	disc_test = []
	for i in range(len(cost_square)):
		row_rest = []
		for j in range(len(cost_square)):
			dist_temp = (coordenates[0][i] - coordenates[0][j])**2 + (coordenates[1][i] - coordenates[1][j])**2
			dist = int(dist_temp.real + 0.5)
			row_rest.append(dist)
		disc_test.append(row_rest)

	print("\nMatriz de costos cuadrátiva\n")
	print("Original:")
	print(np.array(cost_square))
	print("\nGenerada:")
	print(np.array(disc_test))
	'''
	##################################

	return list_coords


def read_instances(instance):
	file = open(os.path.dirname(__file__) + "/../inputs/" + instance + ".txt")

	global num_trucks
	global trucks_capacities
	global num_milks
	global milk_request
	global milk_values
	global num_farms
	global coordenates
	global types_farms
	global milk_farms

	num_trucks = int(file.readline())
	trucks_capacities = file.readline()[:-1].split("\t")
	file.readline()

	num_milks = int(file.readline())
	milk_request = file.readline()[:-1].split("\t")
	milk_values = file.readline()[:-1].split("\t")
	file.readline()

	num_farms = int(file.readline())

	long_instances = False
	for farm in range(num_farms):
		line = file.readline()[:-1]
		array = list(map(float_convert_array, line.split("\t")))

		if(len(array) == 3):
			types_farms.append(array[1])
			milk_farms.append(array[2])
		elif(len(array) == 5):
			coordenates.append([array[1], array[2]])
			types_farms.append(array[3])
			milk_farms.append(array[4])
			long_instances = True
		else:
			print("Erro al leer instancia")
			break

	file.readline()
	if(long_instances):
		for x in range(num_farms):
			row = []
			for y in range(num_farms):
				temp_cost = int(
					sqrt(
						(coordenates[x][0] - coordenates[y][0])**2 + 
						(coordenates[x][1] - coordenates[y][1])**2
					) + 0.5
				)
				row.append(temp_cost)
			cost_matrix.append(row)
	else:
		for farm in range(num_farms):
			line = file.readline()[:-1]
			array = list(map(int_convert_array, line.split("\t")))
			cost_matrix.append(array)

		coordenates = get_coordenates(cost_matrix)
		

def plot_map(instance, route = None, output = ""):
	colors = ['c', 'm', 'y']
	plt.figure(num=None, figsize=(25, 25), dpi=600, facecolor='w', edgecolor='k')
	plt.xlabel('x')
	plt.ylabel('y')
	
	for i in range(num_farms):
	    if types_farms[i] == -1:
	        scatter(coordenates[i][0], coordenates[i][1], s=60 ,marker='s', c='k')
	    elif types_farms[i] == 0:
	        scatter(coordenates[i][0], coordenates[i][1], s=4 ,marker='o', c='r')
	    elif types_farms[i] == 1:
	        scatter(coordenates[i][0], coordenates[i][1], s=4 ,marker='o', c='g')
	    elif types_farms[i] == 2:
	        scatter(coordenates[i][0], coordenates[i][1], s=4 ,marker='o', c='b')
	    plt.text(coordenates[i][0], coordenates[i][1], i, fontsize=2)
	
	plt.legend(('Planta', 'Granja con Leche A', 'Granja con Leche B', 'Granja con Leche C'))

	if route is not None:
		plt.title('Route: ' + route + ' ' + output)
		route_list = list(map(int, route[1:-1].split(",")))
		
		routex = [coordenates[route_list[0]][0]]
		routey = [coordenates[route_list[0]][1]]

		count_route = -1
		for i in range(1, len(route_list)):
			routex.append(coordenates[route_list[i]][0])
			routey.append(coordenates[route_list[i]][1])

			if route_list[i] == 0:
				plot(routex, routey, colors[milk_by_route[count_route]]+'-', linewidth = .2)
				count_route += 1
				routex = [coordenates[route_list[i]][0]]
				routey = [coordenates[route_list[i]][1]]

	plt.savefig(os.path.dirname(__file__) + "/../outputs/" + instance + "-" + output + ".png")


def analysis_instances(route):
	route_list = list(map(int, route[1:-1].split(",")))
	route_cost = []
	milk_income = [0]*num_milks
	collected_milk = [0]*num_milks

	temp_route = [0]
	temp_cost = 0
	temp_milk = 0
	temp_types = []
	real_types = []
	truck_index = 0
	cumulative_milk = 0
	
	for i in range(1, len(route_list)):
		if route_list[i] == 0:
			temp_route.append(route_list[i])
			if len(temp_types) != 0:
				collected_milk[temp_types[-1]] += temp_milk
			
			temp_cost += cost_matrix[route_list[i-1]][route_list[i]]

			#Capacidad de los camiones:
			if temp_milk > int(trucks_capacities[truck_index]):
				temp_cost += (temp_milk - int(trucks_capacities[truck_index]))*10

			route_cost.append(temp_cost)

			#print(temp_route)
			#print(temp_types)
			#print(real_types)
			if temp_cost != 0 and False:
				print(
					"Costo Ruta: ", int(temp_cost), 
					"\tLeche Camion: ", int(temp_milk), "/", trucks_capacities[truck_index], 
				)
				if trucks_capacities[truck_index] - temp_milk < 0: 
					print("-")

			milk_by_route.append(cumulative_milk)
			temp_route = [0]
			temp_cost = 0
			temp_milk = 0
			truck_index += 1
			cumulative_milk = 0
			temp_types = []
			real_types = []
		else:
			temp_cost += cost_matrix[route_list[i-1]][route_list[i]]

			milk_type = types_farms[route_list[i]]
			milk_income[milk_type] += milk_farms[route_list[i]]
			temp_milk += milk_farms[route_list[i]]
			temp_route.append(route_list[i])

			if milk_type >= cumulative_milk:
				cumulative_milk = milk_type
				temp_types.append(milk_type)
			else:
				temp_types.append(cumulative_milk)
			
			real_types.append(milk_type)

	#Solo impresión
	print()
	for i in range(num_milks):
		print("Requerimiento Planta: ", int(milk_request[i]), "\tLeche recogida", int(collected_milk[i]))
	print()

	
	#Mezcla de Leche en la Planta
	for i in reversed(range(1, num_milks)):
		if int(milk_request[i]) > collected_milk[i]:
			collected_milk[i-1] -= int(milk_request[i]) - collected_milk[i];
			collected_milk[i] = int(milk_request[i]);


	total_income = 0
	total_cost = int(sum(route_cost))
	for i in range(num_milks):
		print("Requerimiento Planta: ", int(milk_request[i]), "\tLeche recogida", int(collected_milk[i]))
		
		total_income += math.ceil(collected_milk[i]*float(milk_values[i]))
		if int(milk_request[i]) > collected_milk[i]:
			total_income -= int((int(milk_request[i]) - collected_milk[i])*float(milk_values[i]))*10

	print("\nIngresos Total: ", total_income, "\tCosto Total: ", total_cost, "\tBeneficio: ", total_income - total_cost, "\n")

	return total_income - total_cost


#Read Args
if len(sys.argv) == 1:
	print("It is need pass the instance name")


#Grafica las Soluciones
elif len(sys.argv) == 2:
	read_instances(sys.argv[1])
	plot_map(sys.argv[1])


#Analiza las Soluciones
elif len(sys.argv) == 3:
	read_instances(sys.argv[1])
	print()
	analysis_instances(sys.argv[2])


#Analiza las Soluciones y setea orden en los camiones
elif len(sys.argv) == 4:
	read_instances(sys.argv[1])
	trucks_capacities = list(map(int, sys.argv[3][1:-1].split(",")))

	print()
	quality = analysis_instances(sys.argv[2])

	plot_map(sys.argv[1], sys.argv[2], str(quality))


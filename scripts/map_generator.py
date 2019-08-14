#!/usr/bin/env python3

import numpy as np
from numpy.linalg import eig
from math import sqrt

#Armar matriz de distancias
coords = [
	[0, 1, 3, 4, 2, -1, -2],
	[0, -1, 1, 2, 3, 4, 0]
]
dist_square = []
for i in range(len(coords[0])):
	row_rest = []
	for j in range(len(coords[0])):
		dist_temp = (coords[0][i] - coords[0][j])**2 + (coords[1][i] - coords[1][j])**2
		dist = int(dist_temp + 0.5)
		row_rest.append(dist)
	dist_square.append(row_rest)

###############################################
cost_matrix = [
	[0, 1, 3, 4, 2, 1],
	[15, 0, 4, 5, 3, 4],
	[17, 23, 0, 2, 8, 9],
	[18, 54, 11, 0, 9, 2],
	[19, 12, 11, 64, 0, 5],
	[41, 59, 38, 97, 73, 0]
]
print(np.array(cost_matrix))
for i in range(len(cost_matrix)):
	for j in range(len(cost_matrix)):
		cost_matrix[j][i] = cost_matrix[i][j]
print()
print(np.array(cost_matrix))
input("----")
###############################################


#Calcular matriz M de coeficientes
M = []
for i in range(len(dist_square)):
	Mi = []
	for j in range(len(dist_square)):
		Mij = dist_square[0][j] + dist_square[i][0] - dist_square[i][j]
		Mi.append(Mij/2)
	M.append(Mi)


#Calculando valores y vectores propios
npM = np.array(M)
values, vectors = eig(npM)


#Obtener vectores de coordenadas
coordenates = []
for i in range(len(values)):
	if values[i] > 0.001:
		coord = sqrt(values[i].real)*vectors.T[i]
		coordenates.append(coord.tolist())

print("Coordenadas:")
print(np.array(coordenates))

#Calcular distancias para comparar
disc_test = []
for i in range(len(dist_square)):
	row_rest = []
	for j in range(len(dist_square)):
		dist_temp = (coordenates[0][i] - coordenates[0][j])**2 + (coordenates[1][i] - coordenates[1][j])**2
		dist = int(dist_temp.real + 0.5)
		row_rest.append(dist)
	disc_test.append(row_rest)


#Comparación
print("\nMatriz de costos cuadrátiva\n")
print("Original:")
print(np.array(dist_square))
print("\nGenerada:")
print(np.array(disc_test))

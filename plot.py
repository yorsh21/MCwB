#!/usr/bin/env python
# coding=utf-8

import pygraphviz as pgv
import math

#Diccionario de nodos y arcos

G = pgv.AGraph()
G.node_attr.update(color='red')
G.edge_attr.update(len='10.0',color='blue')

x = [15, 1, 87, 69, 93, 33, 71, 29, 93, 55, 23, 19, 57, 5, 65, 69, 3, 19, 21, 67, 41, 19, 15, 79, 19, 27, 29, 25, 59, 27, 21, 61, 15, 31, 71, 91]
y = [19, 49, 25, 65, 91, 31, 61, 9, 7, 47, 13, 47, 63, 95, 43, 1, 25, 91, 81, 91, 23, 75, 79, 47, 65, 49, 17, 65, 51, 95, 91, 83, 83, 87, 41, 21]
data = [0, 34, 15, 26, 24, 7, 2, 0, 8, 21, 6, 30, 33, 20, 5, 17, 31, 12, 35, 1, 16, 4, 0, 13, 32, 19, 27, 3, 22, 29, 23, 14, 11, 28, 18, 10, 25, 9, 0]

for i in range(len(data)):
	for j in range(i, len(data)):
		if i != j:
			distance = int(math.sqrt((x[data[i]]-x[data[j]])**2 + (y[data[i]]-y[data[j]])**2))
			G.add_edge(str(data[i]), str(data[j]),label=str(distance))

'''
G.add_edge('a','b',label='8')
G.add_edge('a','c',label='2')
G.add_edge('a','e',label='6')
G.add_edge('a','d',label='4')
G.add_edge('b','c',label='6')
G.add_edge('b','e',label='9')
G.add_edge('b','d',label='12')
G.add_edge('c','e',lebel='5')
G.add_edge('c','d',label='3')
G.add_edge('d','e',label='4')
'''

#print(G)

G.layout()

G.draw('file.png')
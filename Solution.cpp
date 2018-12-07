#include "Solution.h"

Solution::Solution(vector<vector<int>> farms_locates, int milks, int trucks) {
	farms =  farms_locates;
	farms_lenght = (int)farms.size();
	milks_lenght = milks_lenght;
	trucks_lenght = trucks_lenght;

	for (int i = 0; i < milks_lenght; i++) list_milks.push_back(i);
	
	int iterator = 1;
	vector<int> temp_nodes(farms_lenght + trucks_lenght, 0);
	nodes = temp_nodes;
	nodes_lenght = (int)nodes.size();

	for (int i = 0; i < trucks_lenght; ++i)
	{
		int init = rand() % farms_lenght;
		while (farms[init][2] == -1 || find (list_milks.begin(), list_milks.end(), farms[init][2]) == list_milks.end()) {
			init = rand() % farms_lenght;
		}
		list_milks[farms[init][2]] = -1;
		
		int counter = 0;
		vector<int> r = random_int_vector(farms_lenght);
		for (int j = 0; j < farms_lenght; ++j)
		{
			if (farms[init][2] == farms[r[j]][2])
			{
				nodes[iterator + counter] = r[j];
				counter++;
			}
		}
		iterator += counter + 1;
	}
}

int Solution::neighbour_lenght() {
	return 1;
}


float Solution::evaluate() {
	/*float route_cost = 0.0;
	float total_cost = 0.0;
	int milk_income = 0.0;
	int collect_milk = 0;
	int truck_index = 0;
	vector<int> satisfied_cuotes = plant_cuotes;
	int local_quality = farms[nodes[1]][2]; //Tipo de leche de la primera granja

	for (int i = 1; i < nodes_lenght; ++i) {
		vector<int> before_farm = farms[nodes[i-1]];
		vector<int> current_farm = farms[nodes[i]];

		route_cost += sqrt(
			pow(current_farm[0] - before_farm[0], 2) + pow(current_farm[1] - before_farm[1], 2)
		);

		//El nodo actual es la planta
		if(nodes[i] == 0) {
			total_cost += route_cost;
			satisfied_cuotes[local_quality] -= collect_milk;

			//Exceso en la capacidad de los camiones
			if (collect_milk > truck_capacities[truck_index]) {
				total_cost += (collect_milk - truck_capacities[truck_index])*10;
			}

			if(i+1 < nodes_lenght) {
				route_cost = 0;
				collect_milk = 0;
				truck_index++;
				local_quality = farms[nodes[i+1]][2];
			}
		}
		else {
			collect_milk += current_farm[3];

			if(milk_values[local_quality] > milk_values[current_farm[2]]) {
				local_quality = current_farm[2];
			}
		}
	}

	for (int i = 0; i < milks_lenght; ++i) {
		milk_income += (plant_cuotes[i] - satisfied_cuotes[i])*milk_values[i];
		
		//Penalización por cuota faltante
		if(satisfied_cuotes[i] >= 0) {
			milk_income -= satisfied_cuotes[i]*milk_values[i]*10;
		}
	}

	return milk_income - total_cost;*/
	return 1.0;
}


float Solution::get_neighbour(int index) {
	return 1.2;
};
float Solution::set_neighbour(int index) {
	return 1.2;
};


vector<int> Solution::random_int_vector(int lenght) {
	vector<int> int_vector(lenght, 0);

	int index = 1;
	while(index < lenght) {
		int i = rand() % lenght;
		
		if(int_vector[i] == 0) {
			int_vector[i] = index;
			index++;
		}
	}
	return int_vector;
}
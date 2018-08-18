#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "Solution.h"

void Solution::init(vector<int> capacities, vector<float> values, vector<vector<int>> locates) {
	truck_lenght = capacities.size();
	farm_lenght = locates.size();
	truck_capacities = capacities;
	milk_values = values;
	farms_locates = locates;

	//print_farms_locates();
}

void Solution::build() {
	vector<int> trucks;
	for (int i = 0; i < truck_lenght; i++) 
		trucks.push_back(i);

	//Agregando la planta como punto de partida
	representation.push_back(0);

	//buscar granja mas cercana a la planta
	int current_node = nearest_node(0);
	int current_milk_type = farms_locates[current_node][2];

	//Agregando nodo más cercano a la planta
	trucks.erase(trucks.begin() + current_milk_type);
	representation.push_back(current_node);
	
	//Agregando más nodos a la solución
	while(farm_lenght + 2 > (int)representation.size()) {
		int temp_node = nearest_node_same_milk(current_node, current_milk_type);

		//Camión lleno, regresa a la planta
		if(temp_node == -1){
			if (trucks.size() == 0){
				break;
			}
			else{
				trucks.erase(trucks.begin() + current_milk_type);
				current_milk_type = trucks[0];
			}
			representation.push_back(0);
			current_node = nearest_node(0);
			if(current_node == -1){
				break;
			}
		} //Camión con capacidad disponible, sigue recorriendo
		else {
			current_node = temp_node;
			representation.push_back(current_node);
		}
	}

	//Agregando la planta como punto de término
	representation.push_back(0);

	//print_int_vector(representation);
	//cout << representation.size() << endl;
	//cout << evaluate(representation) << endl;
}

float Solution::evaluate(vector<int> solution) {
	vector<int> milk_truck;
	vector<int> distance_truck;
	int current_truck = 0;
	int local_milk = 0;
	float distance = 0;

	for (int i = 1; i < (int)solution.size(); ++i)
	{
		local_milk += farms_locates[solution[i]][3];
		distance += sqrt(
			pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2)
				+ 
			pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
		);

		if(solution[i] == 0) {
			milk_truck.push_back(local_milk);
			distance_truck.push_back(distance);
			local_milk = 0;
			distance = 0;
			current_truck++;
		}
	}

	float total_route = 0;
	for (int i = 0; i < (int)distance_truck.size(); ++i)
	{
		total_route += distance_truck[i];
		if(milk_truck[i] > truck_capacities[i]) 
			total_route += (milk_truck[i] - truck_capacities[i])*10;
	}

	return total_route;
}

void Solution::detail_evaluate(vector<int> solution) {
	vector<int> milk_truck;
	vector<int> capacity_extra;
	vector<int> distance_truck;
	int current_truck = 0;
	int local_milk = 0;
	int sum_distances = 0;
	float distance = 0;

	for (int i = 1; i < (int)solution.size(); ++i)
	{
		local_milk += farms_locates[solution[i]][3];
		distance += sqrt(
			pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2)
				+ 
			pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
		);

		if(solution[i] == 0) {
			milk_truck.push_back(local_milk);
			capacity_extra.push_back(local_milk - truck_capacities[current_truck]);
			distance_truck.push_back(distance);
			sum_distances += distance;
			local_milk = 0;
			distance = 0;
			current_truck++;
		}
	}

	cout << "Capacidad de cada camión: ";
	print_int_vector(truck_capacities);
	cout << "Leche que lleva cada camión: ";
	print_int_vector(milk_truck);
	cout << "Sobrepeso de cada camión: ";
	print_int_vector(capacity_extra);
	cout << "Costo por distancias de cada camión: ";
	print_int_vector(distance_truck);
	cout << "Total costos: " << sum_distances << endl;
}

void Solution::myopic() {

}

void Solution::move(int index1, int index2) {

}


/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

void Solution::hill_climbing(int restarts) {
	vector<int> best_solution;
	float quality_best = 1000000;

	for (int i = 0; i <= restarts; ++i) {
		bool local = false;
		int neighbour_index = 0;
		vector<int> solution = random_solution();
		float quality = evaluate(solution);
		float neighbour_quality = 0;

		while(!local) {
			if(neighbour_index <= farm_lenght) {
				neighbour_index++;

				vector<int> new_neighbour = neighbour(solution, neighbour_index);
				neighbour_quality = evaluate(new_neighbour);

				if(neighbour_quality < quality) { //Minimize
					solution = new_neighbour;
					quality = neighbour_quality;
					neighbour_index = 0;
				}
			}
			else {
				local = true;
			}
		}

		if(quality < quality_best) {
			best_solution = solution;
			quality_best = quality;
			cout << quality_best << endl;
		}

		//print_int_vector(solution);
		//cout << "Restart: " << i << endl;
	}
	cout << "Finish algorithm" << endl;

	print_int_vector(best_solution);
	detail_evaluate(best_solution);
	cout << "Calidad: " << quality_best << endl;


}

vector<int> Solution::neighbour(vector<int> solution, int identity) {
	/*vector<int> zeros;
	//find first zero
	for (int i = 0; i < solution.size(); ++i)
	{
		if(solution[i] == 0) {
			zeros.push_back(i);
		}
	}*/

	if(identity > 0 && identity < (int)solution.size()-2) {
		int temp = solution[identity];
		solution[identity] = solution[identity+1];
		solution[identity+1] = temp;
	}

	return solution;
}

vector<int> Solution::random_solution() {
	vector<int> solution(farm_lenght + truck_lenght, 0);

	int index = 1;
	while(index < farm_lenght) {
		int i = rand() % (farm_lenght+1) + 1;
		
		if(solution[i] == 0) {
			solution[i] = index;
			index++;
		}
		else {
			continue;
		}
	}
	return solution;
}



/************************************************************/
/************************ Find Nodes ************************/
/************************************************************/

int Solution::nearest_node(int node) {
	int node_closer = -1;
	int best_distance = 0;
	int temp_distance = 0;
	for (int i = 0; i < farm_lenght; ++i)
	{
		//Verifica que el nodo no este agregado a 'representation'
		if(find(representation.begin(), representation.end(), i) == representation.end()) {
			temp_distance = distance_between_nodes(node, i);
			if(temp_distance > best_distance)
				best_distance = temp_distance;
			node_closer = i;
		}
	}
	return node_closer;
}

int Solution::nearest_node_same_milk(int node, int milk) {
	int node_closer = -1;
	int best_distance = 0;
	int temp_distance = 0;
	for (int i = 0; i < farm_lenght; ++i)
	{
		//Verifica que el nodo tenga el mismo tipo de leche y que no este agregado a 'representation'
		if(same_milk(node, i) && find(representation.begin(), representation.end(), i) == representation.end()) {
			temp_distance = distance_between_nodes(node, i);
			if(temp_distance > best_distance)
				best_distance = temp_distance;
			node_closer = i;
		}
	}
	return node_closer;
}

vector<int> Solution::current_truck_capacity() {
	vector<int> capacity (truck_lenght, 0);
	int temp_capacity = 0;
	int current_truck = 0;
	bool enter = false;
	for (int i = 1; i < (int)representation.size(); ++i)
	{
		temp_capacity += farms_locates[representation[i]][3];
		if(representation[i] == 0) {
			capacity[current_truck] = temp_capacity;
			current_truck++;
			temp_capacity = 0;
			enter = true;
		}
	}

	if (!enter)
		capacity[current_truck] = temp_capacity;

	return capacity;
}


/************************************************************/
/************************ Utilities *************************/
/************************************************************/

void Solution::print_int_vector(vector<int> array) {
	cout << "[";
	for (int i = 0; i < (int)array.size() - 1; ++i)
	{
		cout << array[i] << ", ";
	}
	cout << array[(int)array.size()-1] <<  "]" << endl;
}

void Solution::print_farms_locates() {
	for (int i = 0; i < (int)farms_locates.size(); ++i)
	{
		cout << farms_locates[i][0] << " - " <<  farms_locates[i][1] << " - "  <<  farms_locates[i][2] << " - " <<  farms_locates[i][3]  <<  endl;
	}
	cout << "Total elementos: " << farms_locates.size() << endl << endl;
}


float Solution::distance_between_nodes(int farm1, int farm2) {
	float distance = sqrt(
		pow(farms_locates[farm1][0] - farms_locates[farm1][1], 2)
			+ 
		pow(farms_locates[farm2][0] - farms_locates[farm2][1], 2)
	);
	return distance;
}

bool Solution::same_milk(int farm1, int farm2) {
	if(farms_locates[farm1][2] == farms_locates[farm2][2])
		return true;
	else
		return false;
}
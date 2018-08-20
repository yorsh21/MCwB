#include <cmath>
#include <algorithm>
#include <cstdlib>
#include "Solver.h"

void Solver::init(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes) {
	truck_lenght = capacities.size();
	farm_lenght = locates.size();
	plant_cuotes = cuotes;
	truck_capacities = capacities;
	milk_values = values;
	farms_locates = locates;

	//print_farms_locates();
}

float Solver::evaluate(vector<int> solution, bool show) {
	int route_cost = 0;
	int income_milk = 0;
	int current_truck = 0;
	int local_quality = 100;
	int collect_milk = 0;
	vector<int> milk_trunk(truck_lenght, 0);
	vector<int> distance_truck;

	for (int i = 1; i < (int)solution.size(); ++i) {
		route_cost += sqrt(
			pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2) + 
			pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
		);

		collect_milk += farms_locates[solution[i]][3];
		if(solution[i] != 0 && local_quality > milk_values[farms_locates[solution[i]][2]]) {
			local_quality = farms_locates[solution[i]][2];
		}

		if(solution[i] == 0) {
			distance_truck.push_back(route_cost);
			milk_trunk[local_quality] = collect_milk;

			route_cost = 0;
			local_quality = 100;
			collect_milk = 0;
			current_truck++;
		}
	}

	//Revisión de Costos
	route_cost = 0;
	for (int i = 0; i < (int)distance_truck.size(); ++i) {
		route_cost += distance_truck[i];

		//Penalización por sobrepeso en camiones
		if (milk_trunk[i] > truck_capacities[i])
			route_cost += (milk_trunk[i] - truck_capacities[i])*10;
	}

	//Revisión de Beneficios
	for (int i = 0; i < (int)milk_trunk.size(); ++i) {
		//No se ha cumplido la cuota
		if(plant_cuotes[i] - milk_trunk[i] >= 0) {
			//Beneficios por cuota parcial cumplida
			income_milk += milk_trunk[i]*milk_values[i];

			//Penalización por cuota faltante
			income_milk -= (plant_cuotes[i] - milk_trunk[i])*milk_values[i]*10;
		}
		else { //Se ha sobrepasado la cuota
			income_milk += plant_cuotes[i]*milk_values[i];
		}
	}

	if(show) {
		cout << endl << "Costo por distancias de cada camión: ";
		print_int_vector(distance_truck);
		cout << "Total costos: " << route_cost << endl;

		cout << endl << "Cuotas de la Planta: ";
		print_int_vector(plant_cuotes);
		cout << "Leche llevada por los camiones: ";
		print_int_vector(milk_trunk);
		cout << "Capacidad de cada camión: ";
		print_int_vector(truck_capacities);

		cout << "Total beneficios: " << income_milk << endl;
	}

	return income_milk - route_cost;
}



/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

vector<int> Solver::hill_climbing(int restarts) {
	vector<int> best_solution;
	float quality_best = -1000000;

	for (int i = 0; i <= restarts; ++i) {
		bool local = false;
		int neighbour_index = 0;
		vector<int> solution = random_solution();
		float quality = evaluate(solution, false);
		float neighbour_quality = 0;

		while(!local) {
			if(neighbour_index <= farm_lenght) {
				neighbour_index++;

				vector<int> new_neighbour = neighbour(solution, neighbour_index);
				neighbour_quality = evaluate(new_neighbour, false);

				if(neighbour_quality > quality) { //Minimize
					solution = new_neighbour;
					quality = neighbour_quality;
					neighbour_index = 0;
				}
			}
			else {
				local = true;
			}
		}

		if(quality > quality_best) {
			best_solution = solution;
			quality_best = quality;
			cout << quality_best << endl;
		}

	}
	cout << "Finish local search" << endl;

	//print_int_vector(best_solution);
	//evaluate(best_solution, true);

	return best_solution;
}

vector<int> Solver::neighbour(vector<int> solution, int identity) {
	if(identity > 0 && identity < (int)solution.size()-2) {
		int temp = solution[identity];
		solution[identity] = solution[identity+1];
		solution[identity+1] = temp;
	}

	return solution;
}

vector<int> Solver::random_solution() {
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
/************************ Utilities *************************/
/************************************************************/

void Solver::print_int_vector(vector<int> array) {
	cout << "[";
	for (int i = 0; i < (int)array.size() - 1; ++i)
	{
		cout << array[i] << ", ";
	}
	cout << array[(int)array.size()-1] <<  "]" << endl;
}

void Solver::print_float_vector(vector<float> array) {
	cout << "[";
	for (int i = 0; i < (int)array.size() - 1; ++i)
	{
		cout << array[i] << ", ";
	}
	cout << array[(int)array.size()-1] <<  "]" << endl;
}

void Solver::print_farms_locates() {
	for (int i = 0; i < (int)farms_locates.size(); ++i)
	{
		cout << farms_locates[i][0] << " - " <<  farms_locates[i][1] << " - "  <<  farms_locates[i][2] << " - " <<  farms_locates[i][3]  <<  endl;
	}
	cout << "Total elementos: " << farms_locates.size() << endl << endl;
}

void Solver::export_result(vector<int> solution, string filename) {
	string file = "outputs/" + filename.replace(filename.end()-3, filename.end(), "out");
	cout << "Writing output to: " << file << endl;

	ofstream myfile;
	myfile.open (file);

	if (myfile.is_open()) {
		int route_cost = 0;
		int income_milk = 0;
		int current_truck = 0;
		int local_quality = 100;
		int collect_milk = 0;
		vector<int> milk_trunk(truck_lenght, 0);
		vector<int> distance_truck;
		vector<string> letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
		vector<string> output;
		string local_output = "0-";

		for (int i = 1; i < (int)solution.size(); ++i) {
			route_cost += sqrt(
				pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2) + 
				pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
			);

			local_output += to_string(solution[i]) + "-";

			collect_milk += farms_locates[solution[i]][3];
			if(solution[i] != 0 && local_quality > milk_values[farms_locates[solution[i]][2]]) {
				local_quality = farms_locates[solution[i]][2];
			}

			if(solution[i] == 0) {
				local_output += to_string(solution[i]) + "0 " + to_string(route_cost) + " " + to_string(collect_milk) + letters[local_quality];
				output.push_back(local_output);
				local_output = "0-";

				distance_truck.push_back(route_cost);
				milk_trunk[local_quality] = collect_milk;

				route_cost = 0;
				local_quality = 100;
				collect_milk = 0;
				current_truck++;
			}
		}

		route_cost = 0;
		for (int i = 0; i < (int)distance_truck.size(); ++i) {
			route_cost += distance_truck[i];
			if (milk_trunk[i] > truck_capacities[i])
				route_cost += (milk_trunk[i] - truck_capacities[i])*10;
		}

		for (int i = 0; i < (int)milk_trunk.size(); ++i) {
			if(plant_cuotes[i] - milk_trunk[i] >= 0) {
				income_milk += milk_trunk[i]*milk_values[i];
				income_milk -= (plant_cuotes[i] - milk_trunk[i])*milk_values[i]*10;
			}
			else {
				income_milk += plant_cuotes[i]*milk_values[i];
			}
		}

		string head_output = to_string(income_milk - route_cost) + " " + to_string(route_cost) + " " + to_string(income_milk);
		myfile << head_output << endl;
		for (int i = 0; i < (int)output.size(); ++i) {
			myfile << output[i] << endl;
		}

		cout << "Successful file writing" << endl;
	}
	else {
		cout << "Error when writing the file:" << file << endl;
	}
	myfile.close();
	
}


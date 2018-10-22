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
	float route_cost = 0.0;
	float total_cost = 0.0;
	float income_milk = 0.0;
	int local_quality = 100;
	int collect_milk = 0.0;
	vector<int> milk_trunk(truck_lenght, 0);
	vector<float> distance_truck;

	int len_sol = (int)solution.size();
	for (int i = 1; i < len_sol; ++i) {
		route_cost += sqrt(
			pow(farms_locates[solution[i]][0] - farms_locates[solution[i-1]][0], 2) + 
			pow(farms_locates[solution[i]][1] - farms_locates[solution[i-1]][1], 2)
		);
		cout << route_cost << endl;

		collect_milk += farms_locates[solution[i]][3];
		if(solution[i] != 0 && local_quality > milk_values[farms_locates[solution[i]][2]]) {
			local_quality = farms_locates[solution[i]][2];
		}

		if(solution[i] == 0 && local_quality != 100) {
			//if (collect_milk > truck_capacities[local_quality])
			//	route_cost += (collect_milk - truck_capacities[local_quality])*10;

			distance_truck.push_back(route_cost);
			milk_trunk[local_quality] = collect_milk;

			total_cost += route_cost;
			route_cost = 0;
			local_quality = 100;
			collect_milk = 0;
		}
	}

	/*//Revisión de Costos
	route_cost = 0;
	int len_trun = (int)distance_truck.size();
	for (int i = 0; i < len_trun; ++i) {
		route_cost += distance_truck[i];

		//Penalización por sobrepeso en camiones
		if (milk_trunk[i] > truck_capacities[i])
			route_cost += (milk_trunk[i] - truck_capacities[i])*10;
	}*/

	//Revisión de Beneficios
	int len_milk = (int)milk_trunk.size();
	for (int i = 0; i < len_milk; ++i) {
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
		print_float_vector(distance_truck);
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

float Solver::fast_evaluate(vector<int> solution, float before_eval, int index) {
	float new_eval = 0.0;

	if(solution[index] == 0 || solution[index+1] == 0) {
		new_eval = evaluate(solution, false);
	}
	else {
		float adds = sqrt(
			pow(farms_locates[solution[index-1]][0] - farms_locates[solution[index]][0], 2) + 
			pow(farms_locates[solution[index-1]][1] - farms_locates[solution[index]][1], 2)
		) + 
		sqrt(
			pow(farms_locates[solution[index+1]][0] - farms_locates[solution[index+2]][0], 2) + 
			pow(farms_locates[solution[index+1]][1] - farms_locates[solution[index+2]][1], 2)
		);

		float dels = sqrt(
			pow(farms_locates[solution[index-1]][0] - farms_locates[solution[index+1]][0], 2) + 
			pow(farms_locates[solution[index-1]][1] - farms_locates[solution[index+1]][1], 2)
		) +
		sqrt(
			pow(farms_locates[solution[index]][0] - farms_locates[solution[index+2]][0], 2) + 
			pow(farms_locates[solution[index]][1] - farms_locates[solution[index+2]][1], 2)
		);

		new_eval = before_eval - (adds - dels);

		int old_eval = (int)evaluate(solution, false);
		int renew_eval = (int)new_eval;
		if(old_eval != renew_eval)
			cout << renew_eval << " | " << old_eval << endl;
	}

	return new_eval;
}



/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

vector<int> Solver::hill_climbing(int restarts) {
	clock_t begin = clock();

	vector<int> best_solution;
	float quality_best = -9999999;

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
				//neighbour_quality = fast_evaluate(new_neighbour, quality, neighbour_index);
				neighbour_quality = evaluate(new_neighbour, true);

				if(neighbour_quality > quality) {
					solution = new_neighbour;
					quality = neighbour_quality;
					neighbour_index = 0;
				}
			}
			else {
				local = true;
			}
			print_int_vector(solution);
			break;
		}

		if(quality > quality_best) {
			best_solution = solution;
			quality_best = quality;
			//cout << quality_best << endl;
		}

	}

	clock_t end = clock();

	result_times.push_back(float(end - begin) / CLOCKS_PER_SEC);
	result_qualities.push_back(quality_best);

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
		float route_cost = 0.0;
		float income_milk = 0.0;
		int local_quality = 100;
		int collect_milk = 0.0;
		vector<int> milk_trunk(truck_lenght, 0);
		vector<float> distance_truck;
		vector<string> letters = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
		vector<string> output;
		string local_output = "0-";

		for (int i = 1; i < (int)solution.size(); ++i) {
			route_cost += sqrt(
				pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2) + 
				pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
			);

			if(solution[i] == 0)
				local_output += to_string(solution[i]);
			else
				local_output += to_string(solution[i]) + "-";

			collect_milk += farms_locates[solution[i]][3];
			if(solution[i] != 0 && local_quality > milk_values[farms_locates[solution[i]][2]]) {
				local_quality = farms_locates[solution[i]][2];
			}

			if(solution[i] == 0) {
				local_output += "\t" + to_string(route_cost) + "\t" + to_string(collect_milk) + letters[local_quality];
				output.push_back(local_output);
				local_output = "0-";

				distance_truck.push_back(route_cost);
				milk_trunk[local_quality] = collect_milk;

				route_cost = 0.0;
				local_quality = 100;
				collect_milk = 0;
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

		string head_output = to_string(income_milk - route_cost) + "\t" + to_string(route_cost) + "\t\t" + to_string(income_milk);
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

void Solver::save_row_result(string filename) {
	string file = "outputs/results.out";
	ofstream myfile;
	myfile.open (file, std::fstream::app);

	float best = -9999999;
	float sum = 0;
	float times = 0;
	int len = (int)result_qualities.size();
	for (int i = 0; i < len; ++i)
	{
		sum += result_qualities[i];
		times += result_times[i];
		if(result_qualities[i] > best)
			best = result_qualities[i];
	}

	if (myfile.is_open()) {
		string output = filename + " " + to_string((int)sum/len) + " " + to_string((int)best) + " " + to_string((int)times);
		myfile << output << endl;
	}
	else {
		cout << "Error when writing the file:" << file << endl;
	}
	
	myfile.close();
}


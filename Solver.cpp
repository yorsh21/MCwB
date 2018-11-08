#include "Solver.h"

void Solver::init(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes) {
	trucks_lenght = capacities.size();
	milks_lenght = values.size();
	farms_lenght = locates.size();
	plant_cuotes = cuotes;
	truck_capacities = capacities;
	milk_values = values;
	farms_locates = locates;
}

float Solver::evaluate(vector<int> solution) {
	float route_cost = 0.0;
	float total_cost = 0.0;
	int milk_income = 0.0;
	int collect_milk = 0;
	int truck_index = 0;
	vector<int> satisfied_cuotes = plant_cuotes;
	int local_quality = farms_locates[solution[1]][2]; //Tipo de leche de la primera granja
	bool factible = true;

	int len_sol = (int)solution.size();
	for (int i = 1; i < len_sol; ++i) {
		vector<int> before_farm = farms_locates[solution[i-1]];
		vector<int> current_farm = farms_locates[solution[i]];

		route_cost += sqrt(
			pow(current_farm[0] - before_farm[0], 2) + pow(current_farm[1] - before_farm[1], 2)
		);

		//El nodo actual es la planta
		if(solution[i] == 0) {
			total_cost += route_cost;
			satisfied_cuotes[local_quality] -= collect_milk;

			//Exceso en la capacidad de los camiones
			if (collect_milk > truck_capacities[truck_index]) {
				total_cost += (collect_milk - truck_capacities[truck_index])*10;
				factible = false;
			}

			if(i+1 < len_sol) {
				route_cost = 0;
				collect_milk = 0;
				truck_index++;
				local_quality = farms_locates[solution[i+1]][2];
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
			//cout << "Leche " << i+1 << ": " << satisfied_cuotes[i]*milk_values[i]*10 << endl;
			factible = false;
		}
	}

	/*cout << "Solucion:" << endl;
	print_int_vector(solution);

	cout << endl << "Total:" << milk_income << " - " << total_cost << " = " << milk_income - total_cost << endl << endl;

	cout << "Cuotas Planta:" << endl;
	print_int_vector(plant_cuotes);
	cout << endl << "Cuotas Satisfechas:" << endl;
	print_int_vector(satisfied_cuotes);
	cout << endl;*/

	return milk_income - total_cost;
}

float Solver::fast_evaluate(vector<int> solution, float before_eval, int index) {
	float new_eval = 0.0;

	if(solution[index] == 0 || solution[index+1] == 0) {
		new_eval = evaluate(solution);
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
	}

	return new_eval;
}



/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

vector<int> Solver::hill_climbing(int restarts, int times) {
	clock_t begin = clock();

	vector<int> best_solution;
	float quality_best = -9999999;

	for (int i = 0; i <= restarts; ++i) {
		bool local = false;
		int neighbour_index = 0;
		vector<int> solution = random_feasible_solution();//random_solution();
		float quality = evaluate(solution);
		float neighbour_quality = 0;

		while(!local) {
			if(neighbour_index <= farms_lenght) {
				neighbour_index++;

				vector<int> new_neighbour = neighbour(solution, neighbour_index);
				//neighbour_quality = fast_evaluate(new_neighbour, quality, neighbour_index);
				neighbour_quality = evaluate(new_neighbour);

				if(neighbour_quality > quality) {
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
			//print_int_vector(solution);
		}

		if(float(clock() - begin) / CLOCKS_PER_SEC >= times) {
			break;
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

vector<int> Solver::random_feasible_solution() {
	vector<int> milks;
	for (int i = 0; i < milks_lenght; i++) milks.push_back(i);
	
	int itetator = 1;
	vector<int> solution(farms_lenght + trucks_lenght, 0);
	for (int i = 0; i < trucks_lenght; ++i)
	{
		int init = rand() % farms_lenght;
		while (farms_locates[init][2] == -1 || find (milks.begin(), milks.end(), farms_locates[init][2]) == milks.end()) {
			init = rand() % farms_lenght;
		}
		milks[farms_locates[init][2]] = -1;
		
		int counter = 0;
		vector<int> r = random_int_vector(farms_lenght);
		for (int j = 0; j < farms_lenght; ++j)
		{
			if (farms_locates[init][2] == farms_locates[r[j]][2])
			{
				solution[itetator + counter] = r[j];
				counter++;
			}
		}
		itetator += counter + 1;
	}

	return solution;
}


vector<int> Solver::random_solution() {
	vector<int> solution(farms_lenght + trucks_lenght, 0);

	int index = 1;
	while(index < farms_lenght) {
		int i = rand() % (farms_lenght+1) + 1;
		
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


vector<int> Solver::random_int_vector(int lenght) {
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
		float milk_income = 0.0;
		int local_quality = 100;
		int collect_milk = 0.0;
		vector<int> milk_trunk(trucks_lenght, 0);
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
				milk_income += milk_trunk[i]*milk_values[i];
				milk_income -= (plant_cuotes[i] - milk_trunk[i])*milk_values[i]*10;
			}
			else {
				milk_income += plant_cuotes[i]*milk_values[i];
			}
		}

		string head_output = to_string(milk_income - route_cost) + "\t" + to_string(route_cost) + "\t\t" + to_string(milk_income);
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


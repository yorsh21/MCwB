#include "Solver.h"


Solver::Solver(vector<int> capacities, vector<float> values, vector<vector<int>> locates, vector<int> cuotes, string instance) {
	trucks_lenght = capacities.size();
	milks_lenght = values.size();
	farms_lenght = locates.size();
	plant_cuotes = cuotes;
	truck_capacities = capacities;
	milk_values = values;
	farms_locates = locates;
	name_instance = instance;
	global_quality = 0;

	for (int i = 0; i < farms_lenght; ++i)
	{
		vector<int> row(farms_lenght, 0);
		for (int j = 0; j < farms_lenght; ++j)
		{
			row[j] = sqrt(
				pow(farms_locates[i][0] - farms_locates[j][0], 2) + 
				pow(farms_locates[i][1] - farms_locates[j][1], 2)
			) + 0.5;
		}
		cost_matrix.push_back(row);
	}
	//print_cost_matrix();
}

int Solver::evaluate(vector<int> solution, bool show = false) {
	int route_cost = 0;
	int total_cost = 0;
	int collect_milk = 0;
	int truck_index = 0;
	vector<int> milks_trucks = truck_capacities;
	vector<int> satisfied_cuotes = plant_cuotes;
	int local_quality = farms_locates[solution[1]][2]; //Tipo de leche de la primera granja

	int len_sol = (int)solution.size();
	for (int i = 1; i < len_sol; ++i) {
		vector<int> current_farm = farms_locates[solution[i]];
		route_cost += cost_matrix[solution[i]][solution[i-1]];

		//El nodo actual es la planta
		if(solution[i] == 0) {
			total_cost += route_cost;
			milks_trucks[truck_index] -= collect_milk;

			//Exceso en la capacidad de los camiones
			if (milks_trucks[truck_index] < 0) {
				total_cost -= milks_trucks[truck_index]*100;
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
			satisfied_cuotes[current_farm[2]] -= current_farm[3];

			if(milk_values[local_quality] > milk_values[current_farm[2]]) {
				local_quality = current_farm[2];
			}
		}
	}

	int milk_income = 0;
	for (int i = 0; i < milks_lenght; ++i) {
		milk_income += (plant_cuotes[i] - satisfied_cuotes[i])*milk_values[i] + 0.5;
		
		//Penalización por cuota faltante
		if(satisfied_cuotes[i] >= 0) {
			milk_income -= satisfied_cuotes[i]*milk_values[i]*100;
		}
	}

	if (show) {
		cout << endl << "Total: " << milk_income << " - " << total_cost << " = " << milk_income - total_cost << endl << endl;

		cout << "Capacidad Camiones: ";
		print_int_vector(truck_capacities);
		cout << "Capacidad Restante: ";
		print_int_vector(milks_trucks);

		cout << endl;
		
		cout << "Cuotas Planta:    ";
		print_int_vector(plant_cuotes);
		cout << "Cuotas Faltantes: ";
		print_int_vector(satisfied_cuotes);

		cout << endl;
		draw_graph(solution, milk_income - total_cost);
	}

	return milk_income - total_cost;
}

int Solver::fast_evaluate(vector<int> solution, float before_eval, int index) {
	int new_eval = 0.0;

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

vector<int> Solver::hill_climbing(int end_time) {
	clock_t begin = clock();

	vector<int> best_solution = random_feasible_solution();;
	int quality_best = evaluate(best_solution);

	while (float(clock() - begin) / CLOCKS_PER_SEC < end_time) {
		vector<int> solution = random_feasible_solution();
		int quality = evaluate(solution);
		int neighbour_quality = 0;

		int index = rand() % (farms_lenght - 1);
		vector<int> neighbour = neighbour_index(solution, index);
		for (int i = 0; i < (int)neighbour.size(); ++i)
		{
			//vector<int> new_neighbour = long_swap(solution, index, neighbour[i]);
			//vector<int> new_neighbour = short_swap(solution, neighbour[i]);
			vector<int> new_neighbour = two_opt(solution, index, neighbour[i]);
			neighbour_quality = evaluate(new_neighbour);

			if(neighbour_quality > quality) {
				solution = new_neighbour;
				quality = neighbour_quality;
			}
		}

		if(quality > quality_best) {
			best_solution = solution;
			quality_best = quality;
			cout << ">>>>>>>>>>>>>>>>>>>>>>>> " << name_instance << ": " << quality_best << " <<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

			if (quality > 0) {
				//draw_graph(solution, quality);
				//print_int_vector(solution);
			}
		}
	}

	clock_t end = clock();

	result_times.push_back(float(end - begin) / CLOCKS_PER_SEC);
	result_qualities.push_back(quality_best);

	if(quality_best > global_quality) {
		global_quality = quality_best;
		global_solution = best_solution;
	}

	return best_solution;
}

vector<int> Solver::short_swap(vector<int> solution, int identity) {
	if(identity > 0 && identity < (int)solution.size()-2) {
		int temp = solution[identity];
		solution[identity] = solution[identity+1];
		solution[identity+1] = temp;
	}

	return solution;
}

vector<int> Solver::long_swap(vector<int> solution, int index, int move) {
	if(move > 0 && move < (int)solution.size()-2) {
		int temp = solution[move];
		solution[move] = solution[index];
		solution[index] = temp;
	}

	return solution;
}

vector<int> Solver::two_opt(vector<int> solution, int index1, int index2) {
	if (index1 < index2)
	{
		int diff = index2 - index1;
		for (int i = 0; i < diff; ++i)
		{
			if (index1 + i == index2 - i){
				break;
			}
			else  {
				int temp = solution[index1 + i];
				solution[index1 + i] = solution[index2 - i];
				solution[index2 - i] = temp;
			}

		}
	}
	else if(index1 > index2) 
	{
		int diff = index1 - index2;
		for (int i = 0; i < diff; ++i)
		{
			if (index2 + i == index1 - i){
				break;
			}
			else  {
				int temp = solution[index2 + i];
				solution[index2 + i] = solution[index1 - i];
				solution[index1 - i] = temp;
			}

		}
	}

	return solution;
}

vector<int> Solver::neighbour_index(vector<int> solution, int index) {
	vector<int> indexes;
	int count = 0;
	int len = (int)solution.size();

	while(index + count < len && solution[index + count] != 0) {
		indexes.push_back(index + count);
		count++;
	}
	while(index - count > 0 && solution[index - count] != 0) {
		indexes.push_back(index - count);
		count++;
	}

	return indexes;
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

void Solver::print_cost_matrix() {
	for (int i = 0; i < (int)cost_matrix.size(); ++i)
	{
		string row = "-";
		for (int j = 0; j < (int)cost_matrix.size(); ++j)
		{
			row += to_string((int)cost_matrix[i][j])  + "-";
		}
		cout << row <<  endl;
	}
	cout << endl;
}

string Solver::int_vector_to_string(vector<int> array) {
	string output = "[";
	int len = (int)array.size() - 1;
	for (int i = 0; i < len; ++i)
	{
		output += to_string(array[i]) + ", ";
	}
	output += to_string(array[len]) + "]";

	return output;
}


/************************************************************/
/************************** Exports *************************/
/************************************************************/

void Solver::export_result(vector<int> solution) {
	string file = "outputs/" + name_instance + ".out";
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
		string full_output = "python3 plot.py " + name_instance + " [0";

		for (int i = 1; i < (int)solution.size(); ++i) {
			route_cost += sqrt(
				pow(farms_locates[solution[i]][0] - farms_locates[solution[i]][1], 2) + 
				pow(farms_locates[solution[i-1]][0] - farms_locates[solution[i-1]][1], 2)
			);

			if(solution[i] == 0) {
				local_output += to_string(solution[i]);
			}
			else{
				local_output += to_string(solution[i]) + "-";
			}
			full_output += "," + to_string(solution[i]);

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

		full_output += "]";
		cout << full_output << endl;
		system(full_output.c_str());

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

void Solver::save_row_result() {
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
		string output = name_instance + "\t" + 
			to_string((int)sum/len) + "\t" + 
			to_string((int)best) + "\t" + 
			to_string((int)times) + "\n" + 
			int_vector_to_string(global_solution);
		myfile << output << endl;
	}
	else {
		cout << "Error when writing the file:" << file << endl;
	}
	
	myfile.close();
	cout << "Successfully wrhite instance: " << name_instance << endl;
}

void Solver::draw_graph(vector<int> solution, int quality) {
	string full_output = "python3 plot.py " + name_instance + " [0";

	for (int i = 1; i < (int)solution.size(); ++i) {
		full_output += "," + to_string(solution[i]);
	}
	full_output += "] " + to_string(quality);
	cout << full_output << endl;
	system(full_output.c_str());
}


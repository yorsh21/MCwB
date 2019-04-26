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
	global_quality = -9999999;

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
	int local_quality = farms_locates[solution[1]][2]; //Tipo de leche de la primera granja
	int len_sol = (int)solution.size();
	bool temp_ot = true;

	//Global variables
	remaining_capacity = truck_capacities;
	satisfied_cuotes = plant_cuotes;
	pivots = {};
	quality_by_route = {};
	onetype = {};

	for (int i = 1; i < len_sol; ++i) {
		vector<int> current_farm = farms_locates[solution[i]];
		route_cost += cost_matrix[solution[i]][solution[i-1]];

		//El nodo actual es la planta
		if(solution[i] == 0) {
			total_cost += route_cost;
			remaining_capacity[truck_index] -= collect_milk;
			satisfied_cuotes[local_quality] -= collect_milk;
			pivots.push_back(i);
			quality_by_route.push_back(local_quality);
			onetype.push_back(temp_ot);

			//Exceso en la capacidad de los camiones
			if (remaining_capacity[truck_index] < 0) {
				total_cost -= remaining_capacity[truck_index]*100;
			}

			if(i+1 < len_sol) {
				route_cost = 0;
				collect_milk = 0;
				truck_index++;
				local_quality = farms_locates[solution[i+1]][2];
				temp_ot = true;
			}
		}
		else {
			collect_milk += current_farm[3];

			if(milk_values[local_quality] > milk_values[current_farm[2]]) {
				local_quality = current_farm[2];
				temp_ot = false;
			}
		}
	}

	//Mezcla de Leche en la Planta
	for (int i = milks_lenght - 1; i > 0 ; --i) {
		if(satisfied_cuotes[i] > 0) {
			satisfied_cuotes[i-1] += satisfied_cuotes[i];
			satisfied_cuotes[i] = 0;
		}
	}

	int milk_income = 0;
	for (int i = 0; i < milks_lenght; ++i) {
		milk_income += (plant_cuotes[i] - satisfied_cuotes[i])*milk_values[i] + 0.5;
		
		//Penalización por cuota faltante
		if(satisfied_cuotes[i] > 0) {
			milk_income -= satisfied_cuotes[i]*milk_values[i]*100;
		}
	}


	if (show) {
		cout << endl << "Total: " << milk_income << " - " << total_cost << " = " << milk_income - total_cost << endl << endl;

		cout << "Capacidad Camiones: ";
		print_int_vector(truck_capacities);
		cout << "Capacidad Restante: ";
		print_int_vector(remaining_capacity);

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


int Solver::fast_evaluate(vector<int> solution, int old_eval, int index1, int index2) {
	int new_eval = old_eval;

	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	new_eval += cost_matrix[solution[k1]][solution[k1-1]];
	new_eval += cost_matrix[solution[k2]][solution[k2+1]];
	
	new_eval -= cost_matrix[solution[k2]][solution[k1-1]];
	new_eval -= cost_matrix[solution[k1]][solution[k2+1]];

	return new_eval;
}


int Solver::random_index(vector<int> solution) {
	int index = 0;
	while(solution[index] == 0) {
		index = rand() % (farms_lenght + trucks_lenght - 2) + 1;
	}

	return index;
}


/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

vector<int> Solver::hill_climbing(int end_time) {
	auto start = chrono::system_clock::now();

	vector<int> neighbour;
	vector<int> new_neighbour;
	vector<int> best_solution = random_feasible_solution();
	int quality_best = evaluate(best_solution);
	bool local;

	chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - start;

	//Loop restarts
	while (elapsed_seconds.count() < end_time) 
	{
		vector<int> solution = random_feasible_solution();
		int quality = evaluate(solution);
		int neighbour_quality = 0;

		//Busqueda Local Entre Rutas
		local = false;
		while(!local) {
			local = true;

			for (int g = 0; g < trucks_lenght; ++g)
			{
				vector<vector<int>> vector_routes = intelligence_split_route(solution, g);
				vector<int> select_route = vector_routes[0];
				vector<int> others_route = vector_routes[1];

				int select_size = (int)select_route.size();
				int others_size = (int)others_route.size();

				for (int h = 0; h < select_size; ++h)
				{
					for (int i = 0; i < others_size; ++i)
					{
						if(can_move_extra_routes(solution, select_route[h], others_route[i])) {
							new_neighbour = move_extra_routes(solution, select_route[h], others_route[i]);
							neighbour_quality = evaluate(new_neighbour);
							
							if(neighbour_quality > quality) {
								solution = new_neighbour;
								quality = neighbour_quality;

								if(quality > quality_best) {
									global_trucks_position = truck_capacities;
								}
								
								local = false;
								break;
							}
						}
					}
					//if(!local) break;
				}
				//if(!local) break;
			}
		}


		//Busqueda Completa Intra Rutas
		local = false;
		while(!local) {
			local = true;

			vector<vector<int>> vector_routes = split_routes(solution);
			int len_routes = (int)vector_routes.size();
			for (int g = 0; g < len_routes; ++g)
			{
				int len_route = (int)vector_routes[g].size();
				for (int i = 0; i < len_route; ++i)
				{
					for (int j = 0; j < len_route; ++j)
					{
						neighbour_quality = fast_evaluate(solution, quality, vector_routes[g][i], vector_routes[g][j]);
						if(neighbour_quality > quality) {
							solution = two_opt(solution, vector_routes[g][i], vector_routes[g][j]);
							quality = neighbour_quality;

							if(quality > quality_best) {
								global_trucks_position = truck_capacities;
							}

							local = false;
							break;
						}
					}
					//if(!local) break;
				}
				//if(!local) break;
			}
		}


		if ((int)global_trucks_position.size() == trucks_lenght) {
			truck_capacities = global_trucks_position;
		}

		auto end = chrono::system_clock::now();
		elapsed_seconds = end - start;

		if(quality > quality_best) {
			best_solution = solution;
			quality_best = quality;

			cout << name_instance << ": " << (int)elapsed_seconds.count() << "s  ->  " << quality_best << endl;
			//print_int_vector(solution);
		}
	}

	if(quality_best > global_quality) {
		global_quality = quality_best;
		global_solution = best_solution;
	}

	result_times.push_back((int)elapsed_seconds.count());
	result_qualities.push_back(quality_best);

	return best_solution;
}


vector<int> Solver::short_swap(vector<int> solution, int index) {
	if(index > 0 && index < (int)solution.size()-2) {
		int temp = solution[index];
		solution[index] = solution[index+1];
		solution[index+1] = temp;
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
	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	int diff = (k2 - k1)/2;
	for (int i = 0; i <= diff; ++i)
	{
		int temp = solution[k1 + i];
		solution[k1 + i] = solution[k2 - i];
		solution[k2 - i] = temp;
	}

	return solution;
}


vector<int> Solver::move_extra_routes(vector<int> solution, int index1, int index2) {
	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	int temp = solution[k1];
	for (int i = k1; i < k2; ++i) {
		solution[i] = solution[i+1];
	}
	solution[k2] = temp;

	return solution;
}


vector<int> Solver::neighbour_2opt_index(vector<int> solution, int index) {
	vector<int> indexes;
	int len = (int)solution.size();

	int count = 1;
	while(index + count < len && solution[index + count] != 0) {
		indexes.push_back(index + count);
		count++;
	}

	count = 1;
	while(index - count > 0 && solution[index - count] != 0) {
		indexes.push_back(index - count);
		count++;
	}

	return indexes;
}


vector<int> Solver::neighbour_move_index(vector<int> solution, int index) {
	vector<int> indexes;
	int len = (int)solution.size();

	int route1 = -1;
	int route2 = -1;
	for (int i = 0; i < len; ++i) {
		if (solution[i] == 0) {
			if (route1 != -1 && route2 == -1 && i >= index){
				route2 = i;
			}

			if (i < index) {
				route1 = i;
			}
		}
	}

	for (int i = 1; i < route1; ++i) {
		if (solution[i] != 0) {
			indexes.push_back(i);
		}
	}

	for (int i = route2; i < len; ++i) {
		if (solution[i] != 0) {
			indexes.push_back(i);
		}
	}

	return indexes;
}


vector<int> Solver::random_feasible_solution() {
	//Desordenando camiones
	truck_capacities = random_assignment(truck_capacities);
	
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


vector<int> Solver::get_node_from_route(vector<int> solution, int number) {
	vector<int> nodes = {};

	int init = (number == 0) ? 0 : pivots[number];
	for (int i = init; i < pivots[number]; ++i)
	{
		nodes.push_back(solution[i]);
	}

	return nodes;
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


vector<int> Solver::random_assignment(vector<int> array) {
	int lenght = (int)array.size();
	vector<int> int_vector(lenght, 0);

	int index = 0;
	while(index < lenght) {
		int i = rand() % lenght;
		
		if(int_vector[i] == 0) {
			int_vector[i] = array[index];
			index++;
		}
	}
	return int_vector;
}


vector<vector<int>> Solver::split_routes(vector<int> solution) {
	vector<vector<int>> routes;
	int len = (int)solution.size();

	vector<int> route;
	for (int i = 0; i < len; ++i)
	{
		if (solution[i] == 0) {
			routes.push_back(route);
			route = {};
		}
		else {
			route.push_back(i);
		}
	}

	return routes;
}


vector<vector<int>> Solver::split_route(vector<int> solution, int number) {
	vector<int> into_route;
	vector<int> extra_route;

	int init = (number == 0) ? 0 : pivots[number];
	for (int i = init; i < pivots[number]; ++i)
	{
		if (i > init && i < pivots[number])
			into_route.push_back(solution[i]);
		else
			extra_route.push_back(solution[i]);

	}

	return {into_route, extra_route};
}


vector<vector<int>> Solver::intelligence_split_route(vector<int> solution, int index) {
	vector<int> into_route;
	vector<int> extra_route;
	int len_pivots = (int)pivots.size();

	for (int i = 0; i < len_pivots; ++i) {
		if (quality_by_route[index] < quality_by_route[i]) 
		{
			int init = (i == 0) ? 1 : pivots[i-1]+1;

			for (int j = init; j < pivots[i]; ++j) {
				extra_route.push_back(j);
			}
		}
		else if (quality_by_route[index] == quality_by_route[i])
		{
			int init = (i == 0) ? 1 : pivots[i-1]+1;

			for (int j = init; j < pivots[i]; ++j) {
				into_route.push_back(j);
			}
		}
	}

	return {into_route, extra_route};
}


bool Solver::can_move_extra_routes(vector<int> solution, int index1, int index2) {
	int len_pivots = (int)pivots.size();
	int destination_route = 0;

	//Busca el numero de la ruta del index
	for (int i = 0; i < len_pivots - 1; ++i) {
		if (pivots[i] < index2 && index2 < pivots[i+1]){
			destination_route = i + 1;
			break;
		}
	}

	if (remaining_capacity[destination_route] < farms_locates[solution[index2]][3]) {
		return false;
	}
	else {
		return true;
	}
}


/************************************************************/
/************************ Utilities *************************/
/************************************************************/

void Solver::print_int_vector(vector<int> array) {
	cout << "[";
	for (int i = 0; i < (int)array.size() - 1; ++i)
	{
		cout << array[i] << ",";
	}
	if ((int)array.size() > 0) {
		cout << array[(int)array.size()-1] <<  "]" << endl;
	}
	else {
		cout << "]" << endl;
	}
}


void Solver::print_float_vector(vector<float> array) {
	cout << "[";
	for (int i = 0; i < (int)array.size() - 1; ++i)
	{
		cout << array[i] << ",";
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
	int len = (int)array.size();
	if (len == 0) {
		return "[]";
	} 
	else {
		string output = "[";
		for (int i = 0; i < len-1; ++i) {
			output += to_string(array[i]) + ",";
		}
		output += to_string(array[len]) + "]";

		return output;
	}
}


string Solver::time() {
	auto end = chrono::system_clock::now();

	time_t end_time = chrono::system_clock::to_time_t(end);

	return (string)ctime(&end_time);
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

	int best = -9999999;
	int sum = 0;
	int times = 0;
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
			to_string((int)times) + "\t" + 
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
	full_output += "] " + to_string(quality) + " [" + to_string(truck_capacities[0]);

	for (int i = 1; i < (int)truck_capacities.size(); ++i) {
		full_output += "," + to_string(truck_capacities[i]);
	}
	full_output += "]";

	cout << full_output << endl;
	system(full_output.c_str());
}

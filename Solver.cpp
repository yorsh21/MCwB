#include "Solver.h"


Solver::Solver(Instances instance, string file_name, int x_cap, int x_req, float dist)
{
	x_capacity = x_cap;
	x_request = x_req;
	disturbing = dist;

	trucks_lenght = instance.truck_lenght; //100
	milks_lenght = instance.milk_lenght; //3
	farms_lenght = instance.nodes_lenght; //501

	//print(trucks_lenght);
	//print(milks_lenght);
	//print(farms_lenght);

	plant_cuotes = instance.plant_cuotes; //[1250000,300000,100000]
	truck_capacities = instance.truck_capacities; //[15000,15000,15000,15000,15000,15000,15000 ....]
	milk_values = instance.milk_values; //[0.015,0.0105,0.0045]

	//print_vector(plant_cuotes);
	//print_vector(truck_capacities);
	//print_vector(milk_values);

	farms_types = instance.farms_types; //[-1,1,0,0,0,0,2,0,0,0,0,1,0,0,0,0,0,2 .... ]
	farms_milk = instance.farms_milk; //[0,1070,714,433,5904,3390,3741 ...]
	farms_by_milk = instance.farms_by_milk; //[ 2 3 4 5 7 8 9 10 12 13 14 15 ... ] [ ... ] [ ... ]

	//print_vector(farms_types);
	//print_vector(farms_milk);
	//print_matrix(farms_by_milk);

	min_index_value = 0;
	for (int i = 1; i < milks_lenght; ++i)
	{
		if(milk_values[i] < milk_values[min_index_value])
			min_index_value = i;
	}

	name_instance = file_name;
	global_quality = -999999999;

	cost_matrix = instance.cost_matrix;

	//print_matrix(cost_matrix);
}


int Solver::evaluate(vector<vector<int>> solution, bool show = false)
{
	int route_cost = 0;
	int total_cost = 0;
	int collect_milk = 0;
	int local_quality = 0;

	//Global variables
	remaining_capacity = truck_capacities;
	satisfied_cuotes = plant_cuotes;
	quality_by_route = {};
	
	//Calculando Costos de las Rutas
	for (int i = 0; i < trucks_lenght; ++i)
	{
		vector<int> row = solution[i];
		int row_len = (int)row.size();
		
		if(row_len > 0) 
		{
			row.push_back(0); //No se almacena el aumento en el largo en row_len

			collect_milk = 0; //La última granja siempre es la planta: sin leche
			route_cost = cost_matrix[0][row[0]];
			local_quality = farms_types[row[0]];

			for (int j = 0; j < row_len; ++j)
			{
				collect_milk += farms_milk[row[j]];
				route_cost += cost_matrix[row[j]][row[j+1]];

				//Recalculado calidad de la leche de esta ruta
				if(milk_values[local_quality] > milk_values[farms_types[row[j]]]) {
					local_quality = farms_types[row[j]];
				}
			}

			total_cost += route_cost;
			remaining_capacity[i] -= collect_milk;
			satisfied_cuotes[local_quality] -= collect_milk;

			quality_by_route.push_back(local_quality);

			//Exceso en la capacidad de los camiones
			if (remaining_capacity[i] < 0) {
				total_cost -= remaining_capacity[i]*x_capacity;
			}
		}
		else
		{
			quality_by_route.push_back(min_index_value);
		}
	}

	//Mezcla de Leche en la Planta
	for (int i = milks_lenght - 1; i > 0 ; --i) {
		if(satisfied_cuotes[i] > 0) {
			satisfied_cuotes[i-1] += satisfied_cuotes[i];
			satisfied_cuotes[i] = 0;
		}
	}

	//Calculo de Beneficio
	int milk_income = 0;
	for (int i = 0; i < milks_lenght; ++i) {
		milk_income += (plant_cuotes[i] - satisfied_cuotes[i])*milk_values[i] + 0.5;

		//Penalización por cuota faltante
		if(satisfied_cuotes[i] > 0) {
			milk_income -= satisfied_cuotes[i]*milk_values[i]*x_request;
		}
	}


	if (show) {
		cout << endl << "Total: " << milk_income << " - " << total_cost << " = " << milk_income - total_cost << endl << endl;

		cout << "Cuotas Planta: " << endl;
		print_vector(plant_cuotes);
		print_vector(satisfied_cuotes);

		cout << endl << "Capacidad Camiones: ";
		print_vector(truck_capacities);
		print_vector(remaining_capacity);

		//export_solution(solution);
	}

	return milk_income - total_cost;
}


int Solver::fast_evaluate(vector<int> row, int eval, int index1, int index2)
{
	row.insert(row.begin(), 0);
	row.push_back(0);

	int k1 = min(index1, index2) + 1;
	int k2 = max(index1, index2) + 1;

	//Quitar Costos
	for (int i = k1; i <= k2 + 1; ++i)
	{
		eval += cost_matrix[row[i-1]][row[i]];
	}

	//Agregar Costos
	for (int i = k1 + 1; i <= k2; ++i)
	{
		eval -= cost_matrix[row[i]][row[i-1]];
	}

	eval -= cost_matrix[row[k1-1]][row[k2]];
	eval -= cost_matrix[row[k1]][row[k2+1]];

	return eval;
}


/************************************************************/
/********************* Búsqueda Local  **********************/
/************************************************************/

vector<vector<int>> Solver::iteration_local_search(int end_time, int max_quality)
{
	//auto start = chrono::system_clock::now();
	//chrono::duration<double> elapsed_seconds;
	
	clock_t t;
	t = clock();
	int elapsed_seconds = 0;

	vector<vector<int>> neighbour;
	vector<vector<int>> solution;
	vector<vector<int>> best_solution;
	vector<int> best_trucks = truck_capacities;

	int quality;
	int neighbour_quality = global_quality;
	int quality_best = global_quality;

	bool local;
	bool supreme_local = false;


	//Loop ILS
	do
	{
		solution = supreme_local ? disturbing_solution(solution) : random_feasible_solution2();
		quality = evaluate(solution);
		supreme_local = false;

		while(!supreme_local) 
		{
			supreme_local = true;

			//Busqueda Local Extra Rutas
			local = false;
			while(!local) {
				local = true;

				int index_origin = rand() % trucks_lenght;
				for (int gg = 0; gg < trucks_lenght; ++gg) //Origen
				{
					int g = (index_origin + gg) % trucks_lenght;
					int index_destination = rand() % trucks_lenght;

					for (int hh = 0; hh < trucks_lenght; ++hh) //Destino
					{
						int h = (index_destination + hh) % trucks_lenght;
						int route_len = (int)solution[g].size();

						if(g != h && route_len != 0) {
							int index_nodo = rand() % route_len;

							for (int ii = 0; ii < route_len; ++ii) //Nodo origen
							{
								int i = (index_nodo + ii) % route_len;

								if(feasible_movement(solution[g][i], h)) {
									neighbour = move_extra_routes(solution, g, h, i);
									neighbour_quality = evaluate(neighbour);

									if(neighbour_quality > quality) {
										solution = neighbour;
										quality = neighbour_quality;

										local = false;
										supreme_local = false;

										break; 
									}
								}
							}
						}
					}
				}
			}

			//Busqueda Local Intra Rutas
			local = false;
			while(!local) {
				local = true;

				int index_ruta = rand() % trucks_lenght;
				for (int gg = 0; gg < trucks_lenght; ++gg) //Ruta
				{
					int g = (index_ruta + gg) % trucks_lenght;
					int row_len = (int)solution[g].size();

					if(row_len != 0) {
						int index_init = rand() % row_len;

						for (int hh = 0; hh < row_len; ++hh) //Nodo inicio
						{
							int h = (index_init + hh) % row_len;
							int index_destination = rand() % row_len;

							for (int ii = 0; ii < row_len; ++ii) //Nodo final
							{
								int i = (index_destination + ii) % row_len;

								if(h != i) {
									neighbour_quality = fast_evaluate(solution[g], quality, h, i);

									if(neighbour_quality > quality) {
										solution = move_intra_routes(solution, g, h, i);
										quality = neighbour_quality;

										local = false;
										supreme_local = false;

										//break;
									}
								}
							}
						}
					}
				}
			}

		} //End suprime while


		//auto end = chrono::system_clock::now();
		//elapsed_seconds = end - start;
		t = clock() - t;
		elapsed_seconds = (int)((float)t)/CLOCKS_PER_SEC;

		if(quality > quality_best) {
			quality_best = quality;
			best_solution = solution;
			best_trucks = truck_capacities;

			cout << name_instance << ": " << elapsed_seconds << "s  ->  " << quality_best << endl;
		}

	} while(elapsed_seconds < end_time && quality_best < max_quality);


	truck_capacities = best_trucks;
	save_thread_result(name_instance + ": " + to_string(elapsed_seconds) + "s  ->  " + to_string(quality_best) + " " + matrix_to_string(best_solution) + " " + vector_to_string(best_trucks));

	return best_solution;
}


vector<vector<int>> Solver::hill_climbing(int end_time, int max_quality)
{
	auto start = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds;

	vector<vector<int>> neighbour;
	vector<vector<int>> solution;
	vector<vector<int>> best_solution;
	vector<int> best_trucks = truck_capacities;

	int quality;
	int neighbour_quality = global_quality;
	int quality_best = global_quality;

	bool local;
	bool supreme_local = false;


	//Loop ILS/HC
	do
	{
		//solution = supreme_local ? disturbing_solution(solution) : random_feasible_solution2();
		solution = random_feasible_solution2();
		quality = evaluate(solution);
		supreme_local = false;

		while(!supreme_local) 
		{
			supreme_local = true;

			//Busqueda Local Extra Rutas
			local = false;
			while(!local) {
				local = true;

				for (int g = 0; g < trucks_lenght; ++g) //Origen
				{
					for (int h = 0; h < trucks_lenght; ++h) //Destino
					{
						if(g != h) {
							for (int i = 0; i < (int)solution[g].size(); ++i) //Nodo origen
							{
								if(feasible_movement(solution[g][i], h)) {
									neighbour = move_extra_routes(solution, g, h, i);
									neighbour_quality = evaluate(neighbour);

									if(neighbour_quality > quality) {
										solution = neighbour;
										quality = neighbour_quality;

										local = false;
										supreme_local = false;

										break;
									}
								}
							}
						}
					}
				}
			}

			//Busqueda Local Intra Rutas
			local = false;
			while(!local) {
				local = true;

				for (int g = 0; g < trucks_lenght; ++g) //Ruta
				{
					int row_len = (int)solution[g].size();
					for (int h = 0; h < row_len; ++h) //Nodo inicio
					{
						for (int i = 0; i < row_len; ++i) //Nodo final
						{
							if(h != i) {
								neighbour_quality = fast_evaluate(solution[g], quality, h, i);

								if(neighbour_quality > quality) {
									solution = move_intra_routes(solution, g, h, i);
									quality = neighbour_quality;

									local = false;
									supreme_local = false;

									break;
								}
							}
						}
					}
				}
			}

		} //End suprime while


		auto end = chrono::system_clock::now();
		elapsed_seconds = end - start;

		if(quality > quality_best) {
			quality_best = quality;
			best_solution = solution;
			best_trucks = truck_capacities;

			cout << name_instance << ": " << (int)elapsed_seconds.count() << "s  ->  " << quality_best << endl;
		}

	} while(elapsed_seconds.count() < end_time && quality_best < max_quality);


	truck_capacities = best_trucks;
	save_thread_result(name_instance + ": " + to_string((int)elapsed_seconds.count()) + "s  ->  " + to_string(quality_best) + " " + matrix_to_string(best_solution) + " " + vector_to_string(best_trucks));

	return best_solution;
}


vector<vector<int>> Solver::move_extra_routes(vector<vector<int>> solution, int index1, int index2, int node)
{
	vector<int> destination = solution[index2];
	destination.insert(destination.begin(), 0);
	destination.push_back(0);

	int element = solution[index1][node];
	int len_destination = (int)destination.size();
	int min_cost = 999999999;
	int min_index = 0;

	for (int i = 0; i < len_destination - 1; ++i)
	{
		int temp_cost = cost_matrix[destination[i]][element] + cost_matrix[element][destination[i+1]];
		if(temp_cost < min_cost) {
			min_cost = temp_cost;
			min_index = i;
		}
	}

	solution[index1].erase(solution[index1].begin() + node);
	solution[index2].insert(solution[index2].begin() + min_index, element);

	return solution;
}


vector<vector<int>> Solver::move_intra_routes(vector<vector<int>> solution, int node, int index1, int index2) 
{
	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	int diff = (k2 - k1)/2;
	for (int i = 0; i <= diff; ++i)
	{
		if(k1 + i != k2 - i) {
			int temp = solution[node][k1 + i];
			solution[node][k1 + i] = solution[node][k2 - i];
			solution[node][k2 - i] = temp;
		}
	}

	return solution;
}


vector<vector<int>> Solver::random_feasible_solution()
{
	truck_capacities = clutter_vector(truck_capacities);
	vector<int> milk_by_truck = truck_capacities;
	vector<int> farms;
	vector<vector<int>> solution;

	for (int i = 1; i < farms_lenght; ++i){
		farms.push_back(i);
	}
	
	for (int i = 0; i < trucks_lenght; ++i){
		solution.push_back({});
	}

	int index_truck = 0;
	while((int)farms.size() > 0)
	{
		int index_farm = rand() % (int)farms.size();
		while(milk_by_truck[index_truck] - farms_milk[farms[index_farm]] > 0)
		{
			solution[index_truck].push_back(farms[index_farm]);
			milk_by_truck[index_truck] -= farms_milk[farms[index_farm]];
			farms.erase(farms.begin() + index_farm);

			if((int)farms.size() > 0)
				index_farm = rand() % (int)farms.size();
			else
				break;
		}

		if(++index_truck >= trucks_lenght) break;
	}

	return solution;
}


vector<vector<int>> Solver::random_feasible_solution2()
{
	truck_capacities = clutter_vector(truck_capacities);
	vector<int> milk_by_truck = truck_capacities;
	vector<int> types_by_truck;
	vector<int> farms;
	vector<vector<int>> solution;

	for (int i = 1; i < farms_lenght; ++i){
		farms.push_back(i);
	}
	
	int offset = rand() % milks_lenght;
	for (int i = 0; i < trucks_lenght; ++i){
		solution.push_back({});
		types_by_truck.push_back((i + offset) % milks_lenght);
	}

	int TOL = 0;
	int index_truck = 0;
	int len_farms = (int)farms.size();
	while((int)farms.size() > 0)
	{
		int index_farm = rand() % (int)farms.size();
		for (int h = 0; h < (int)farms.size(); ++h)
		{
			int i = (index_farm + h) % (int)farms.size();
			while(milk_by_truck[index_truck] - farms_milk[farms[i]] >= TOL && milk_values[types_by_truck[index_truck]] == milk_values[farms_types[farms[i]]])
			{
				solution[index_truck].push_back(farms[i]);
				milk_by_truck[index_truck] -= farms_milk[farms[i]];
				farms.erase(farms.begin() + i);

				if((int)farms.size() > 0)
					i = rand() % (int)farms.size();
				else
					break;
			}
		}

		if(++index_truck >= trucks_lenght) {
			index_truck = 0;

			if(len_farms > (int)farms.size()) {
				len_farms = (int)farms.size();
			}
			else {
				for (int j = 0; j < trucks_lenght; ++j)
				{
					if(solution[j].size() == 0) {
						types_by_truck[j] = rand() % milks_lenght;
					}
				}
				TOL -= 10;
			}
		}
	}

	return solution;
}


vector<vector<int>> Solver::disturbing_solution(vector<vector<int>> solution) {
	int deep_disturbing = farms_lenght * disturbing + 0.5;

	for (int i = 0; i < deep_disturbing; ++i)
	{
		int route_1 = rand() % trucks_lenght;
		int route_2 = rand() % trucks_lenght;
		
		int node_1 = solution[route_1].size();
		int node_2 = solution[route_2].size();

		if(node_1 != 0 && node_2 != 0) {
			node_1 = rand() % node_1;
			node_2 = rand() % node_2;

			if(route_1 == route_2) {
				int temp = solution[route_1][node_1];
				solution[route_1][node_1] = solution[route_2][node_2];
				solution[route_2][node_2] = temp;

			}
			else {
				solution[route_2].insert(solution[route_2].begin() + node_2, solution[route_1][node_1]);
				solution[route_1].erase(solution[route_1].begin() + node_1);
			}
		}
	}

	return solution;
}


vector<int> Solver::random_int_vector(int lenght)
{
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


vector<int> Solver::clutter_vector(vector<int> array)
{
	vector<int> int_vector;

	while((int)array.size() > 0) {
		int i = rand() % (int)array.size();

		int_vector.push_back(array[i]);
		array.erase(array.begin() + i);
	}

	return int_vector;
}


bool Solver::feasible_movement(int index1, int index2)
{
	if (remaining_capacity[index2] >= farms_milk[index1]) 
	{
		if(quality_by_route[index2] >= farms_types[index1]) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


void Solver::vector_map_milk_types(vector<vector<int>> solution)
{
	for (int i = 0; i < trucks_lenght; ++i)
	{
		vector<int> row = solution[i];

		for (int j = 0; j < (int)row.size(); ++j) {
			row[j] = farms_types[row[j]];
		}

		print_vector(row);
	}
}

void Solver::string_map_milk_types(vector<vector<int>> solution)
{
	cout << "| ";
	for (int i = 0; i < trucks_lenght; ++i)
	{
		for (int j = 0; j < (int)solution[i].size(); ++j) {
			cout << farms_types[solution[i][j]] << " ";
		}
		cout << "| ";
	}
	cout << endl;
}


/************************************************************/
/************************ Utilities *************************/
/************************************************************/

void Solver::print(int element)
{
	cout << element << endl;
}


void Solver::print(float element)
{
	cout << element << endl;
}


void Solver::print(string element) {
	cout << element << endl;
}


void Solver::print_vector(vector<int> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}


void Solver::print_vector(vector<float> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}


void Solver::print_vector(vector<string> array)
{
	int len = (int)array.size();

	if (len == 0) {
		cout << "[]" << endl;
	}
	else {
		cout << "[";
		for (int i = 0; i < len - 1; ++i) {
			cout << array[i] << ",";
		}

		cout << array[len - 1] <<  "]" << endl;
	}
}


void Solver::print_matrix(vector<vector<int>> matrix)
{
	for (int i = 0; i < (int)matrix.size(); ++i)
	{
		string row = "[ ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += to_string(matrix[i][j]) + " ";
		}
		cout << row << "]" <<  endl;
	}
	cout << endl;
}


void Solver::print_matrix(vector<vector<float>> matrix)
{
	for (int i = 0; i < (int)matrix.size(); ++i)
	{
		string row = "[ ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += to_string(matrix[i][j]) + " ";
		}
		cout << row << "]" <<  endl;
	}
	cout << endl;
}


void Solver::print_matrix(vector<vector<string>> matrix)
{
	for (int i = 0; i < (int)matrix.size(); ++i)
	{
		string row = "[ ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += matrix[i][j] + " ";
		}
		cout << row << "]" <<  endl;
	}
	cout << endl;
}


string Solver::matrix_to_string(vector<vector<int>> array)
{
	int len = (int)array.size();

	if (len == 0) {
		return "[]";
	}
	else {
		string output = "[0";
		for (int i = 0; i < len; ++i) 
		{
			int len_row = (int)array[i].size();
			if(len_row > 0) {
				for (int j = 0; j < (int)array[i].size(); ++j) {
					output += "," + to_string(array[i][j]);
				}
				output += ",0";
			}
			else {
				output += ",0";

			}
			
		}
		output += "]";

		return output;
	}
}


string Solver::vector_to_string(vector<int> array)
{
	int len = (int)array.size();

	if (len == 0) {
		return "[]";
	}
	else {
		string output = "[";
		for (int i = 0; i < len - 1; ++i) {
			output += to_string(array[i]) + ",";
		}
		output += to_string(array[len - 1]) + "]";

		return output;
	}
}


string Solver::time()
{
	auto end = chrono::system_clock::now();

	time_t end_time = chrono::system_clock::to_time_t(end);

	return (string)ctime(&end_time);
}


/************************************************************/
/************************** Exports *************************/
/************************************************************/


void Solver::save_thread_result(string text)
{
	string file = "outputs/results_threads.out";
	ofstream myfile;
	myfile.open (file, std::fstream::app);

	while(!myfile.is_open()) {
		this_thread::sleep_for(chrono::milliseconds(500));
		cout << "Waiting for open file instance " << name_instance << endl;
	}

	myfile << text << endl;

	myfile.close();
	//cout << "Successfully wrhite instance: " << name_instance << endl;
}


void Solver::export_solution(vector<vector<int>> solution)
{
	string output = "python3 scripts/plot.py " + name_instance + " " + matrix_to_string(solution) + " " + vector_to_string(truck_capacities);

	cout << endl << output << endl;
	system(output.c_str());
}

#include "Solver.h"


Solver::Solver(Instances instance, string file_name, int x_cap, int x_req)
{
	x_capacity = x_cap;
	x_request = x_req;

	trucks_lenght = instance.truck_lenght;
	milks_lenght = instance.milk_lenght;
	farms_lenght = instance.nodes_lenght;

	//print(trucks_lenght);
	//print(milks_lenght);
	//print(farms_lenght);

	plant_cuotes = instance.plant_cuotes;
	truck_capacities = instance.truck_capacities;
	milk_values = instance.milk_values;

	//print_vector(plant_cuotes);
	//print_vector(truck_capacities);
	//print_vector(milk_values);

	farms_types = instance.farms_types;
	farms_milk = instance.farms_milk;
	farms_by_milk = instance.farms_by_milk;

	//print_vector(farms_types);
	//print_vector(farms_milk);
	//print_matrix(farms_by_milk);

	name_instance = file_name;
	global_quality = -999999999;

	cost_matrix = instance.cost_matrix;

	//print_matrix(cost_matrix);
}


int Solver::evaluate(vector<int> solution, bool show = false)
{
	int route_cost = 0;
	int total_cost = 0;
	int collect_milk = 0;
	int truck_index = 0;
	int local_quality = farms_types[solution[1]]; //Tipo de leche de la primera granja
	int len_sol = (int)solution.size();

	//Global variables
	remaining_capacity = truck_capacities;
	satisfied_cuotes = plant_cuotes;
	pivots = {};
	quality_by_route = {};

	if (show) cout << "Costo Ruta: ";

	//Calculando Costos de las Rutas
	for (int i = 1; i < len_sol; ++i) {
		route_cost += cost_matrix[solution[i]][solution[i-1]];

		//El nodo actual es la planta
		if(solution[i] == 0) {
			total_cost += route_cost;
			remaining_capacity[truck_index] -= collect_milk;
			satisfied_cuotes[local_quality] -= collect_milk;
			pivots.push_back(i);
			quality_by_route.push_back(local_quality);

			if (show) cout << route_cost <<  " ";

			//Exceso en la capacidad de los camiones
			if (remaining_capacity[truck_index] < 0) {
				total_cost -= remaining_capacity[truck_index]*x_capacity;
			}

			if(i+1 < len_sol) {
				route_cost = 0;
				collect_milk = 0;
				truck_index++;
				local_quality = farms_types[solution[i+1]];
			}
		}
		else {
			collect_milk += farms_milk[solution[i]];

			if(milk_values[local_quality] > milk_values[farms_types[solution[i]]]) {
				local_quality = farms_types[solution[i]];
			}
		}
	}

	if (show) cout << endl;

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

		cout << "Capacidad Camiones: ";
		print_vector(truck_capacities);
		cout << "Capacidad Restante: ";
		print_vector(remaining_capacity);

		cout << endl;

		cout << "Cuotas Planta:    ";
		print_vector(plant_cuotes);
		cout << "Cuotas Faltantes: ";
		print_vector(satisfied_cuotes);

		cout << endl;
		//draw_graph(solution, milk_income - total_cost);
	}

	return milk_income - total_cost;
}


int Solver::fast_evaluate_2opt(vector<int> solution, int old_eval, int index1, int index2)
{
	int new_eval = old_eval;

	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	//Quitar Costos
	new_eval += cost_matrix[solution[k1]][solution[k1-1]];
	new_eval += cost_matrix[solution[k2]][solution[k2+1]];

	//Agregar Costos
	new_eval -= cost_matrix[solution[k2]][solution[k1-1]];
	new_eval -= cost_matrix[solution[k1]][solution[k2+1]];

	return new_eval;
}

int Solver::fast_evaluate_swap(vector<int> solution, int old_eval, int index1, int index2)
{
	if(abs(index1 - index2) == 1) {
		return fast_evaluate_2opt(solution, old_eval, index1, index2);
	}
	else {
		int new_eval = old_eval;

		int k1 = min(index1, index2);
		int k2 = max(index1, index2);

		//Quitar Costos
		new_eval += cost_matrix[solution[k1]][solution[k1-1]];
		new_eval += cost_matrix[solution[k1]][solution[k1+1]];
		new_eval += cost_matrix[solution[k2]][solution[k2-1]];
		new_eval += cost_matrix[solution[k2]][solution[k2+1]];

		//Agregar Costos
		new_eval -= cost_matrix[solution[k2]][solution[k1-1]];
		new_eval -= cost_matrix[solution[k2]][solution[k1+1]];
		new_eval -= cost_matrix[solution[k1]][solution[k2-1]];
		new_eval -= cost_matrix[solution[k1]][solution[k2+1]];

		return new_eval;
	}
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

vector<int> Solver::hill_climbing()
{
	auto start = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds;

	vector<int> neighbour;
	vector<int> new_neighbour;
	vector<int> solution;
	vector<int> best_solution;
	vector<int> best_trucks = truck_capacities;

	int quality;
	int neighbour_quality = global_quality;
	int quality_best = global_quality;

	bool local;
	bool supreme_local;


	//Loop restarts
	while (true)
	{
		solution = random_feasible_solution3();
		quality = evaluate(solution);
		supreme_local = false;

		while(!supreme_local) 
		{
			supreme_local = true;

			print("::::::::::::::::::::: EXTRA");


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
							//vector<int> rollback_remaining_capacity = remaining_capacity;
							//vector<int> rollback_truck_capacities = truck_capacities;

							//if(can_move_extra_routes(solution, select_route[h], others_route[i])) {
								new_neighbour = move_extra_routes(solution, select_route[h], others_route[i]);
								neighbour_quality = evaluate(new_neighbour);

								if(neighbour_quality > quality) {
									solution = new_neighbour;
									quality = neighbour_quality;
									cout << "extra " << quality << endl;

									//local = false;
									//supreme_local = false;
								}
								//else {
								//	remaining_capacity = rollback_remaining_capacity;
								//	truck_capacities = rollback_truck_capacities;
								//}
							//}

						}
					}
				}
			}

			//print_vector(solution);

			//print("::::::::::::::::::::: INTRA");


			//Busqueda Completa Intra Rutas 2opt
			/*local = false;
			while(!local) {
				local = true;
				quality = evaluate(solution);

				vector<vector<int>> vector_routes = split_routes(solution);
				int len_routes = (int)vector_routes.size();
				for (int g = 0; g < len_routes; ++g)
				{
					int len_route = (int)vector_routes[g].size();
					for (int i = 0; i < len_route; ++i)
					{
						for (int j = 0; j < len_route; ++j)
						{
							neighbour_quality = fast_evaluate_2opt(solution, quality, vector_routes[g][i], vector_routes[g][j]);
							if(neighbour_quality > quality) {
								solution = two_opt(solution, vector_routes[g][i], vector_routes[g][j]);
								quality = evaluate(solution);//neighbour_quality;
								cout << "extra " << quality << endl;

								local = false;
								supreme_local = false;
							}
						}
					}
				}
			}*/

			print("::::::::::::::::::::: TERMINO");

			/*if(quality < 0)
				break;*/
		}

		auto end = chrono::system_clock::now();
		elapsed_seconds = end - start;

		if(quality > quality_best) {
			quality_best = quality;
			best_solution = solution;
			best_trucks = truck_capacities;

			cout << name_instance << ": " << (int)elapsed_seconds.count() << "s  ->  " << quality_best << endl;
		}

		break;
	}

	print_vector(best_solution);
	evaluate(best_solution, true);


	/*if(quality_best > global_quality) {
		global_quality = quality_best;
		global_solution = best_solution;
	}

	result_times.push_back((int)elapsed_seconds.count());
	result_qualities.push_back(quality_best);*/

	//string text_result = name_instance + ": " + to_string((int)elapsed_seconds.count()) + "s -> " + to_string(quality_best) + " " +  int_vector_to_string(best_solution) + " " +  int_vector_to_string(best_trucks);

	//cout << "::::::::::::::::::: END " << text_result << endl;

	//save_thread_result(text_result);

	return best_solution;
}


vector<int> Solver::improve_solution(vector<int> solution, vector<int> trucks_order)
{
	bool local = false;
	int quality = 0;
	int neighbour_quality = 0;

	truck_capacities = trucks_order;

	while(!local) {
		local = true;
		quality = evaluate(solution);

		vector<vector<int>> vector_routes = split_routes(solution);
		int len_routes = (int)vector_routes.size();
		for (int g = 0; g < len_routes; ++g)
		{
			int len_route = (int)vector_routes[g].size() - 1;
			for (int i = 0; i < (int)len_route/2-1; ++i)
			{
				neighbour_quality = fast_evaluate_2opt(solution, quality, vector_routes[g][i], vector_routes[g][len_route-i]);
				if(neighbour_quality > quality) {
					solution = two_opt(solution, vector_routes[g][i], vector_routes[g][len_route-i]);
					quality = neighbour_quality;

					local = false;
				}
			}
		}
	}

	print(quality);

	return solution;
}


vector<int> Solver::long_swap(vector<int> solution, int index1, int index2)
{
	int temp = solution[index1];

	solution[index1] = solution[index2];
	solution[index2] = temp;

	return solution;
}


vector<int> Solver::two_opt(vector<int> solution, int index1, int index2)
{
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


vector<int> Solver::move_extra_routes(vector<int> solution, int index1, int index2)
{
	int k1 = min(index1, index2);
	int k2 = max(index1, index2);

	int temp = solution[k1+1];
	for (int i = k1+1; i < k2; ++i) {
		solution[i] = solution[i+1];
	}
	solution[k2] = temp;

	return solution;
}


vector<int> Solver::neighbour_2opt_index(vector<int> solution, int index)
{
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


vector<int> Solver::neighbour_move_index(vector<int> solution, int index)
{
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


vector<int> Solver::random_feasible_solution()
{
	//Desordenando camiones
	print_vector(truck_capacities);
	truck_capacities = random_assignment(truck_capacities);
	print_vector(truck_capacities);

	vector<int> milks;
	for (int i = 0; i < milks_lenght; i++) milks.push_back(i);
	print_vector(milks);
	cout << farms_lenght << endl;
	cout << trucks_lenght << endl;

	int itetator = 1;
	vector<int> solution(farms_lenght + trucks_lenght, 0);
	print_vector(solution);
	for (int i = 0; i < trucks_lenght; ++i)
	{
		int init = rand() % farms_lenght;
		while (farms_types[init] == -1 || find (milks.begin(), milks.end(), farms_types[init]) == milks.end()) {
			init = rand() % farms_lenght;
		}
		print(init);
		milks[farms_types[init]] = -1;

		int counter = 0;
		vector<int> r = random_int_vector(farms_lenght);
		for (int j = 0; j < farms_lenght; ++j)
		{
			if (farms_types[init] == farms_types[r[j]])
			{
				solution[itetator + counter] = r[j];
				counter++;
			}
		}
		itetator += counter + 1;
	}

	return solution;
}


vector<int> Solver::random_feasible_solution2()
{
	int first_truck = rand() % milks_lenght;
	int index_truck = 0;
	vector<int> solution = {0};

	while(index_truck < milks_lenght) {
		int i = (first_truck + index_truck) % milks_lenght;

		vector<int> temp_farm_milk = farms_by_milk[i];

		do
		{
			int j = rand() % temp_farm_milk.size();
			solution.push_back(temp_farm_milk[j]);
			temp_farm_milk.erase(temp_farm_milk.begin() + j);

		}
		while (temp_farm_milk.size() > 0);

		solution.push_back(0);

		index_truck++;
	}

	return solution;
}


vector<int> Solver::random_feasible_solution3()
{
	truck_capacities = clutter_vector(truck_capacities);

	int first_truck = rand() % milks_lenght;
	int index_truck = 0;
	int index_solution = 0;
	int farm_by_truck = farms_lenght/trucks_lenght;

	vector<int> solution = {0};

	//Loop tipos de leche: 3 veces
	while(index_truck < milks_lenght) {
		int i = (first_truck + index_truck) % milks_lenght;

		vector<int> temp_farm_milk = farms_by_milk[i];

		while (temp_farm_milk.size() > 0)
		{
			//Loop granjas por camión
			int count = index_solution;
			bool bk = false;
			while(count < farm_by_truck)
			{
				if(temp_farm_milk.size() > 0) {
					int k = rand() % temp_farm_milk.size();
					solution.push_back(temp_farm_milk[k]);
					temp_farm_milk.erase(temp_farm_milk.begin() + k);
				}
				else {
					bk = true;
					break;
				}

				count++;
			}

			if(bk) {
				index_solution = count;
			}
			else {
				index_solution = 0;
				solution.push_back(0);
			}
		}

		index_truck++;
	}

	return solution;
}


vector<int> Solver::get_node_from_route(vector<int> solution, int number)
{
	vector<int> nodes = {};

	int init = (number == 0) ? 0 : pivots[number];
	for (int i = init; i < pivots[number]; ++i)
	{
		nodes.push_back(solution[i]);
	}

	return nodes;
}


vector<int> Solver::random_solution()
{
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


vector<int> Solver::random_assignment(vector<int> array)
{
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


vector<vector<int>> Solver::split_routes(vector<int> solution)
{
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


vector<vector<int>> Solver::split_route(vector<int> solution, int number)
{
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


vector<vector<int>> Solver::intelligence_split_route(vector<int> solution, int index)
{
	vector<int> intra_route;
	vector<int> extra_route;
	int len_solution = (int)solution.size();
	//print_vector(solution);
	//print_vector(pivots);
	//print_vector(quality_by_route);

	//Buscar pivotes contenedores
	int piv1 = (index == 0) ? 0 : pivots[index - 1];
	int piv2 = pivots[index];

	for (int i = 0; i < piv1; ++i) {
		extra_route.push_back(i);
	}

	for (int i = piv1; i < piv2; ++i) {
		if(solution[i] != 0) {
			intra_route.push_back(i);
		}
	}

	for (int i = piv2; i < len_solution - 1; ++i) {
		extra_route.push_back(i);
	}


	print(index);
	print(piv1);
	print(piv2);


	print_vector(intra_route);
	print_vector(extra_route);


	return {intra_route, extra_route};
}


bool Solver::can_move_extra_routes(vector<int> solution, int index1, int index2)
{
	int len_pivots = (int)pivots.size();
	int destination_route = 0;

	//Busca el numero de la ruta del index
	for (int i = 0; i < len_pivots - 1; ++i) {
		if (pivots[i] < index2 && index2 < pivots[i+1]){
			destination_route = i + 1;
			break;
		}
	}

	if (remaining_capacity[destination_route] < farms_milk[solution[index2]]) {
		for (int i = 0; i < (int)truck_capacities.size(); ++i)
		{
			if(i != destination_route &&
				truck_capacities[i] - remaining_capacity[i] <= truck_capacities[destination_route] &&
				truck_capacities[destination_route] - remaining_capacity[destination_route] + farms_milk[solution[index2]] <= truck_capacities[i]
			) {
				//cout << "Swap Camiones: " << destination_route << "<->" << i << endl;
				int temp_remaining = remaining_capacity[destination_route];
				int temp_truck = truck_capacities[destination_route];

				remaining_capacity[destination_route] = remaining_capacity[i];
				truck_capacities[destination_route] = truck_capacities[i];

				remaining_capacity[i] = temp_remaining;
				truck_capacities[i] = temp_truck;

				return true;
			}
		}
		return false;
	}
	else {
		return true;
	}
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
		string row = " ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += to_string(matrix[i][j]) + " ";
		}
		cout << row <<  endl;
	}
	cout << endl;
}


void Solver::print_matrix(vector<vector<float>> matrix)
{
	for (int i = 0; i < (int)matrix.size(); ++i)
	{
		string row = " ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += to_string(matrix[i][j]) + " ";
		}
		cout << row <<  endl;
	}
	cout << endl;
}


void Solver::print_matrix(vector<vector<string>> matrix)
{
	for (int i = 0; i < (int)matrix.size(); ++i)
	{
		string row = " ";
		for (int j = 0; j < (int)matrix[i].size(); ++j)
		{
			row += matrix[i][j] + " ";
		}
		cout << row <<  endl;
	}
	cout << endl;
}


string Solver::int_vector_to_string(vector<int> array)
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

void Solver::save_row_result()
{
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
	//cout << "Successfully wrhite instance: " << name_instance << endl;
}


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


void Solver::draw_graph(vector<int> solution, int quality)
{
	string full_output = "python3 scripts/plot.py " + name_instance + " [0";

	for (int i = 1; i < (int)solution.size(); ++i) {
		full_output += "," + to_string(solution[i]);
	}
	full_output += "] [" + to_string(truck_capacities[0]);

	for (int i = 1; i < (int)truck_capacities.size(); ++i) {
		full_output += "," + to_string(truck_capacities[i]);
	}
	full_output += "]";

	cout << full_output << endl;
	system(full_output.c_str());
}
#include <iostream>
#include <time.h>
#include <string>
#include <iterator>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>
#include <thread>
#include <experimental/filesystem>

#include "Instances.cpp"
#include "Solution.cpp"
#include "Solver0.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

void clean_result() 
{
	string file = "outputs/results.out";
	ofstream myfile;
	myfile.open (file);
	myfile.close();
}

void runInstances(string file_name, int time_running, vector<Solver> *solvers) 
{
	//Leyendo instancias
	Instances instance = Instances();
	instance.read_instances(file_name + ".txt");

	//Creando estructura de las soluciones
	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, file_name);
	
	//Ejecutando algoritmo de búsqueda local
	vector<int> seeds = {1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 1539350253};
	int local_time = (int)(time_running/(int)seeds.size()/10 + 0.5);
	for (int i = 0; i < (int)seeds.size(); ++i)
	{
		srand (seeds[i]);
		sol.hill_climbing(local_time);
		cout << i << ": " << file_name << "  ->  " << sol.global_quality << endl;
	}

	solvers->push_back(sol);
	cout << "--------- Finish " << file_name << endl;
}


void runSeeds(Instances instance, string file_name, int time_running, vector<Solver> *solvers)
{
	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, file_name);
	cout << file_name << ": Running for " << time_running << endl;
	sol.hill_climbing(time_running);

	solvers->push_back(sol);
	cout << file_name << "  ->  " << sol.global_quality << endl;
}

int main(int argc, char *argv[]) 
{
	cout << "==========================================" << endl;
	vector<string> inputs = {"a36", "a62", "a64", "a65", "eil22", "eil51", "eil76", "tai75A"};
	vector<int> times = {110, 1022, 5395, 478, 12, 154, 1700, 4806};

	if(argc == 1) {
		vector<thread> threads;
		vector<Solver> solvers;

		for (int i = 0; i < (int)inputs.size(); ++i){
			threads.push_back(thread(runInstances, inputs[i], times[i], &solvers));
		}

		this_thread::sleep_for(chrono::seconds(400));
		for (int i = 0; i < (int)inputs.size(); ++i){
			cout << "::::::::::::::::::: Joining Thread :::::::::::::::::::" << i << endl;
			threads[i].join();
		}

		//Imprimiento resultados en outputs/result.out
		clean_result();
		for (int i = 0; i < (int)solvers.size(); ++i){
			solvers[i].save_row_result();
		}

	}
	else if(argc == 2) {
		int seed = time(NULL);
		srand (seed);
		string input = argv[1];
		
		//Buscando tiempos
		int time = 0;
		vector<string>::iterator it = find (inputs.begin(), inputs.end(), input);
		if (it != inputs.end()){
			int index = std::distance(inputs.begin(), it);
			time = times[index];
		}

		cout << "Seed: " << seed << endl;
		cout << "Time: " << time << " seconds / " << time/60 << " minutes" << endl << endl;

		//Leyendo instancias
		Instances instance = Instances();
		instance.read_instances(input + ".txt");

		//Creando estructura de las soluciones
		Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, input);

		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(time);
		sol.evaluate(solution, true);

		//Exportando solución
		//sol.export_result(solution);
	}
	else if(argc == 3) {
		int seed = time(NULL);
		srand (seed);
		string input = argv[1];
		int loops = stoi(argv[2]);

		//Buscando tiempos
		int time = 0;
		vector<string>::iterator it = find (inputs.begin(), inputs.end(), input);
		if (it != inputs.end()){
			int index = std::distance(inputs.begin(), it);
			time = times[index];
		}

		cout << "Seed: " << seed << endl;
		cout << "Threads: " << loops << endl;
		cout << "Time: " << time << " seconds / " << time/60 << " minutes" << endl << endl;

		//Leyendo instancias
		Instances instances = Instances();
		instances.read_instances(input + ".txt");

		
		vector<thread> threads;
		vector<Solver> solvers;
		for (int i = 0; i < loops; ++i){
			threads.push_back(thread(runSeeds, instances, input, time, &solvers));
		}

		this_thread::sleep_for(chrono::seconds(time));
		for (int i = 0; i < loops; ++i){
			cout << "::::::::::::::::::: Joining Thread :::::::::::::::::::" << i << endl;
			threads[i].join();
		}
		
		cout << endl;
		for (int i = 0; i < (int)solvers.size(); ++i){
			solvers[i].print_int_vector(solvers[i].global_solution);
		}
	}
	else {
		cout << "Excess of parameters" << endl;
	}
	
	cout << "End algorithm" << endl;
	return 0;
}
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
#include <mutex>
#include <experimental/filesystem>

#include "Instances.cpp"
#include "Solution.cpp"
#include "Solver0.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

vector<Solver> solvers;
mutex mut;

void clean_result() 
{
	string file = "outputs/results.out";
	ofstream myfile;
	myfile.open (file);
	myfile.close();
}

void put_solvers(Solver sol) {
	mut.lock();
	::solvers.push_back(sol);
	mut.unlock();
}

void print(string text) {
	mut.lock();
	cout << text << endl;
	mut.unlock();
}

void runInstances(string file_name, int time_running) 
{
	//Leyendo instancias
	Instances instance = Instances();
	instance.read_instances(file_name + ".txt");

	//Creando estructura de las soluciones
	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, file_name);
	
	//Ejecutando algoritmo de búsqueda local
	vector<int> seeds = {1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 1539350253, 1539344818, 1539344696, 1539344634, 1539344526, 1539344434, 1539344472, 1539343457, 1539342487, 1539342076, 1539340235};
	int loop = (int)seeds.size();
	int local_time = (int)(time_running/loop + 0.5);
	for (int i = 0; i < loop; ++i)
	{
		srand (seeds[i]);
		sol.hill_climbing(local_time);
		print(to_string(i) + ": " + file_name + "  \t-> " + to_string(sol.global_quality));
	}

	put_solvers(sol);
	print("------------ Finish " + file_name + " ------------");
}


void runSeeds(Instances instance, string file_name, int time_running)
{
	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, file_name);
	print(file_name + ": Running for " + to_string(time_running) + " seconds");
	sol.hill_climbing(time_running);

	put_solvers(sol);
	print(file_name + "  ->  " + to_string(sol.global_quality));
}

int main(int argc, char *argv[]) 
{
	cout << "==========================================" << endl;
	vector<string> inputs = {"a36", "a62", "a64", "a65", "eil22", "eil51", "eil76", "tai75A"};
	vector<int> times = {110, 1022, 5395, 478, 12, 154, 1700, 4806};

	if(argc == 1) {
		vector<thread> threads;

		for (int i = 0; i < (int)inputs.size(); ++i){
			threads.push_back(thread(runInstances, inputs[i], times[i]*4));
		}

		this_thread::sleep_for(chrono::seconds(6000));
		for (int i = 0; i < (int)inputs.size(); ++i){
			if(threads[i].joinable()){
				cout << "::::::::::::::::::: Join Thread " << i << " :::::::::::::::::::" << endl;
				threads[i].join();
			}
			else {
				cout << "Thread " << i << "not can Join!" << endl;
			}
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

		vector<int> eil = {0,11,8,2,5,20,17,14,0,16,19,13,4,1,7,10,0,3,6,9,12,15,18,21,0};
		sol.evaluate(eil);
		sol.print_int_vector(eil);
		vector<int> vec = sol.neighbour_move_intelligence(eil, 15);
		sol.print_int_vector(vec);

		return 0;

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
		for (int i = 0; i < loops; ++i){
			threads.push_back(thread(runSeeds, instances, input, time*4));
		}

		this_thread::sleep_for(chrono::seconds(time));
		for (int i = 0; i < loops; ++i){
			if(threads[i].joinable()){
				cout << "::::::::::::::::::: Start Join Thread " << i << " :::::::::::::::::::" << endl;
				threads[i].join();
				cout << "::::::::::::::::::: Finish Join Thread " << i << " :::::::::::::::::::" << endl;
			}
			else {
				cout << "Thread " << i << "not can Join!" << endl;
			}
		}
		
		cout << endl;
		for (int i = 0; i < (int)solvers.size(); ++i){
			cout << solvers[i].global_quality << "\t";
			solvers[i].print_int_vector(solvers[i].global_solution);
		}
	}
	else {
		cout << "Excess of parameters" << endl;
	}
	
	cout << "End algorithm" << endl;
	return 0;
}
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
#include "Solver.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

void clean_result() 
{
	string file = "outputs/results.out";
	ofstream myfile;
	myfile.open (file);
	myfile.close();
}

void run(string file_name, int time_running) 
{
	//Leyendo instancias
	Instances instances = Instances();
	instances.read_instances(file_name + ".txt");

	//Creando estructura de las soluciones
	Solver sol = Solver(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes, file_name);
	
	//Ejecutando algoritmo de búsqueda local
	vector<int> seeds = {1539354881, 1539354669, 1539354643, 1539354562, 1539354443, 1539354427, 1539353575, 1539352478, 1539352067, 1539350253};
	int local_time = (int)(time_running/(int)seeds.size());
	for (int i = 0; i < (int)seeds.size(); ++i)
	{
		srand (seeds[i]);
		sol.hill_climbing(local_time);
		cout << file_name << " " << i << ": " << sol.global_quality << endl;
	}
	
	//Exportando solución
	sol.save_row_result();
}

int main(int argc, char *argv[]) 
{
	cout << "==========================================" << endl;
	vector<string> inputs = {"a36", "a62", "a64", "a65", "eil22", "eil51", "eil76", "tai75A"};
	vector<int> times = {110, 1022, 5395, 478, 12, 154, 1700, 4806};

	if(argc == 1) {
		vector<thread> threads;
		clean_result();

		for (int i = 0; i < (int)inputs.size(); ++i){
			threads.push_back(thread(run, inputs[i], times[i]));
		}

		this_thread::sleep_for(chrono::seconds(15000));
		for (int i = 0; i < (int)inputs.size(); ++i){
			cout << "Joining Thread " << i << endl;
			threads[i].join();
		}
	}
	else if(argc == 2) {
		int seed = time(NULL);
		srand (seed);
		string input = argv[1];
		
		//Buscando tiempos
		int time = 3600;
		vector<string>::iterator it = find (inputs.begin(), inputs.end(), input);
		if (it != inputs.end()){
			int index = std::distance(inputs.begin(), it);
			time = times[index];
		}

		cout << "Seed: " << seed << endl;
		cout << "Time: " << time << " seconds / " << time/60 << " minutes" << endl << endl;

		//Leyendo instancias
		Instances instances = Instances();
		instances.read_instances(input + ".txt");

		//Creando estructura de las soluciones
		Solver sol = Solver(instances.truck_capacities, instances.milk_values, instances.farms_locates, instances.plant_cuotes, input);

		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(time/4);
		
		//Exportando solución
		//sol.export_result(solution);
	}
	else {
		cout << "Excess of parameters" << endl;
	}
	
	cout << "End algorithm" << endl;
	return 0;
}
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
#include "Solver.cpp"

using namespace std;
namespace fs = std::experimental::filesystem;

void pruebas() {
	Instances instance = Instances();
	instance.read_instances("a36.txt");

	srand ((int)time(NULL));

	Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, "a36");

	for (int i = 0; i < 100000; ++i)
	{
		vector<int> solution = sol.random_feasible_solution();
		sol.print_int_vector(solution);
		sol.print(sol.evaluate(solution));
	}
}


int main(int argc, char *argv[]) 
{
	if(argc == 5) 
	{
		string input = argv[1];
		int seed = stoi(argv[2]);
		int time = stoi(argv[3]);
		int quality = stoi(argv[4]);

		srand (seed);
		
		//Leyendo instancias
		Instances instance = Instances();
		instance.read_instances(input + ".txt");

		//Creando estructura de las soluciones
		Solver sol = Solver(instance.truck_capacities, instance.milk_values, instance.farms_locates, instance.plant_cuotes, input);

		//Ejecutando algoritmo de búsqueda local
		vector<int> solution = sol.hill_climbing(time, quality);
		//int eval = sol.evaluate(solution);

		//cout << "::::::::::::::::::: END: " + input + " -> " + to_string(eval) + " :::::::::::::::::::" << endl;
		return 0;
	}
	else {
		//pruebas();
		return 0;
	}
}